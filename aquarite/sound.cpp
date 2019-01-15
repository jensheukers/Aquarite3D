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

#define DEFAULT_DISTANCE_MODEL AL_LINEAR_DISTANCE_CLAMPED
#define DEFAULT_ROLLOF_FACTOR 3.3f
#define DEFAULT_PITCH 1.0f
#define DEFAULT_GAIN 1.0f
#define DEFAULT_MAX_DIST 50.0f
#define DEFAULT_MAX_REF_DIST 5.0f

Sound::Sound() {
	this->source = nullptr;
	this->pitch = 1;
	this->gain = 1;
	this->max_distance = 10.0f; // default is 10.0f
	this->ref_distance = 10.0f; // default is 10.0f;
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

	int endian = 0;
	int bitStream;
	long bytes;
	char array[BUFFER_SIZE];
	FILE* f;

	//Open for binary reading
	f = fopen(aSource->sourcePath.c_str(), "rb");

	if (f == NULL) {
		Debug::Log("Could not load .ogg file", typeid(*this).name());
		return;
	}

	vorbis_info* pInfo;
	OggVorbis_File oggFile;
	ov_open(f, &oggFile, NULL, 0); // Open the ogg file

								   //Get some information about the ogg file
	pInfo = ov_info(&oggFile, -1);

	// Check the number of channels... always use 16-bit samples
	if (pInfo->channels == 1)
		aSource->format = AL_FORMAT_MONO16;
	else
		aSource->format = AL_FORMAT_STEREO16;
	// End if

	//Frequency of the sampling rate
	aSource->freq = pInfo->rate;

	//Decode the data
	do {
		//Read up to a buffers worth of decoded sound data
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
		//Append to end of buffer
		aSource->bufferData.insert(aSource->bufferData.end(), array, array + bytes);
	} while (bytes > 0);

	ov_clear(&oggFile);

	//We have loaded the .ogg file into memory, we can now generate the buffers and buffer the data
	//Generate buffers
	alGenBuffers((ALuint)1, &aSource->bufferID);

	error = alGetError();
	if (error != AL_NO_ERROR) { // If error
		Debug::Log("Could not load audio source : " + error, typeid(*this).name());
		return;
	}
	
	//Buffer the data
	alBufferData(aSource->bufferID, aSource->format, &aSource->bufferData[0], 
				 aSource->bufferData.size() * sizeof(char), aSource->freq);

	error = alGetError();
	if (error != AL_NO_ERROR) { // If error
		Debug::Log("Could not buffer audio data", typeid(*this).name());
		return;
	}

	alSourcei(aSource->sourceID, AL_BUFFER, aSource->bufferID);
	this->source = aSource; // Set the source

	//Set default values
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
	this->SetRollofFactor(DEFAULT_ROLLOF_FACTOR);
	this->SetPitch(DEFAULT_PITCH);
	this->SetGain(DEFAULT_GAIN);
	this->SetMaxDistance(DEFAULT_MAX_DIST); // default is 50.0f
	this->SetMaxReferenceDistance(DEFAULT_MAX_REF_DIST); // default is 1.0f;
	this->SetPosition(Vec3(0,0,0));
	this->SetVelocity(Vec3(0,0,0));
	this->Loop(false);

	Debug::Log("Loaded audio source : " + path, typeid(*this).name());
}

void Sound::SetDistanceModel(ALfloat model) {
	if (this->source == nullptr) return;

	alDistanceModel(model);
	this->distanceModel = model;
}

void Sound::SetRollofFactor(float factor) {
	if (this->source == nullptr) return;

	alSourcef(this->source->sourceID, AL_ROLLOFF_FACTOR, (ALfloat)factor);
	this->rollof_factor = (ALfloat)factor;
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

void Sound::SetMaxDistance(float distance) {
	if (this->source == nullptr) return;

	alSourcef(this->source->sourceID, AL_MAX_DISTANCE, (ALfloat)distance);
	this->max_distance = (ALfloat)distance;
}

ALfloat Sound::GetMaxDistance() {
	return this->max_distance;
}

void Sound::SetMaxReferenceDistance(float distance) {
	if (this->source == nullptr) return;

	alSourcef(this->source->sourceID, AL_REFERENCE_DISTANCE, (ALfloat)distance);
	this->ref_distance = (ALfloat)distance;
}

ALfloat Sound::GetMaxReferenceDistance() {
	return this->ref_distance;
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

void Sound::SetAudioSource(AudioSource* source) {
	this->source = source;
}