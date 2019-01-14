/**
*	Filename: sound.cpp
*
*	Description: Source file for Sound class.
*
*	Version: 13/1/2019
*
*	© 2019, Jens Heukers
*/
#include "core.h"
#include "sound.h"
#include "debug.h"

#define BUFFER_SIZE 32768 // 32 KB buffers

static inline ALenum to_al_format(short channels, short samples)
{
	bool stereo = (channels > 1);

	switch (samples) {
	case 16:
		if (stereo)
			return AL_FORMAT_STEREO16;
		else
			return AL_FORMAT_MONO16;
	case 8:
		if (stereo)
			return AL_FORMAT_STEREO8;
		else
			return AL_FORMAT_MONO8;
	default:
		return -1;
	}
}

bool Sound::LoadOgg(char* fileName, std::vector<char> &buffer, ALenum& format, ALsizei &freq) {
	int endian = 0;
	int bitStream;
	long bytes;
	char array[BUFFER_SIZE];
	FILE* f;

	//Open for binary reading
	f = fopen(fileName, "rb");

	if (f == NULL) {
		return 1;
	}

	vorbis_info* pInfo;
	OggVorbis_File oggFile;
	ov_open(f, &oggFile, NULL, 0); // Open the ogg file

	//Get some information about the ogg file
	pInfo = ov_info(&oggFile, -1);

	// Check the number of channels... always use 16-bit samples
	if (pInfo->channels == 1)
		format = AL_FORMAT_MONO16;
	else
		format = AL_FORMAT_STEREO16;
	// End if

	//Frequency of the sampling rate
	freq = pInfo->rate;

	//Decode the data
	do {
		//Read up to a buffers worth of decoded sound data
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
		//Append to end of buffer
		buffer.insert(buffer.end(), array, array + bytes);
	} while (bytes > 0);

	ov_clear(&oggFile);

	return 0;
}

Sound::Sound() {
	this->source = nullptr;
	this->pitch = 1;
	this->gain = 1;
}

Sound::~Sound() {
	if (this->source == nullptr) return;
	alDeleteSources(1, &this->source->sourceID);
	alDeleteBuffers(1, &this->source->bufferID);
}

void Sound::LoadAudioSource(std::string path) {
	Debug::Log("Loading audio source : " + path, typeid(*this).name());
	
	ALCenum error;

	AudioSource* aSource = new AudioSource(); // Create new audio source struct instance

	aSource->sourcePath = Core::GetBuildDirectory() + path; // Set path
	alGenSources((ALuint)1, &aSource->sourceID); // Generate sources

	error = alGetError();
	if (error != AL_NO_ERROR) { // If error
		Debug::Log("Could not load audio source : " + error, typeid(*this).name());
		return;
	}

	if (Sound::LoadOgg((char*)aSource->sourcePath.c_str(), aSource->bufferData, aSource->format, aSource->format) != 0) {
		Debug::Log("Could not load .ogg file", typeid(*this).name());
		return;
	}

	//Generate buffers
	alGenBuffers((ALuint)1, &aSource->bufferID);

	error = alGetError();
	if (error != AL_NO_ERROR) { // If error
		Debug::Log("Could not load audio source : " + error, typeid(*this).name());
		return;
	}
	
	//Buffer the data
	alBufferData(aSource->bufferID, aSource->format, &aSource->bufferData[0], 
				 static_cast < ALsizei > (aSource->bufferData.size()), aSource->freq);

	error = alGetError();
	if (error != AL_NO_ERROR) { // If error
		Debug::Log("Could not buffer audio data", typeid(*this).name());
		return;
	}

	alSourcei(aSource->sourceID, AL_BUFFER, aSource->bufferID);
	this->source = aSource; // Set the source

	//Set default values
	this->SetPitch(1.0f);
	this->SetGain(1.0f);
	this->SetPosition(Vec3(0,0,0));
	this->SetVelocity(Vec3(0,0,0));
	this->Loop(false);

	Debug::Log("Loaded audio source : " + path, typeid(*this).name());
}

void Sound::SetPitch(float pitch) {
	if (this->source == nullptr) return;

	alSourcef(this->source->sourceID, AL_PITCH, (ALfloat)pitch);
	this->pitch = (ALfloat)pitch;
}

ALfloat Sound::GetPitch() {
	return this->pitch;
}

void Sound::SetGain(float gain) {
	if (this->source == nullptr) return;

	alSourcef(this->source->sourceID, AL_GAIN, (ALfloat)gain);
	this->gain = (ALfloat)gain;
}

ALfloat Sound::GetGain() {
	return this->gain;
}

void Sound::SetPosition(Vec3 pos) {
	if (this->source == nullptr) return;

	alSource3f(this->source->sourceID, AL_POSITION, (ALfloat)pos.x, (ALfloat)pos.y, (ALfloat)pos.z);
	this->position = pos;
}

Vec3 Sound::GetPosition() {
	return this->position;
}

void Sound::SetVelocity(Vec3 vel) {
	if (this->source == nullptr) return;

	alSource3f(this->source->sourceID, AL_VELOCITY, (ALfloat)vel.x, (ALfloat)vel.y, (ALfloat)vel.z);
	this->velocity = velocity;
}

Vec3 Sound::GetVelocity() {
	return this->velocity;
}

void Sound::Loop(bool state) {
	if (this->source == nullptr) return;

	ALboolean s;
	if (state) {
		s = AL_TRUE;
	}
	else {
		s = AL_FALSE;
	}
	alSourcei(this->source->sourceID, AL_LOOPING, s);
}

AudioSource* Sound::GetAudioSource() {
	return this->source;
}