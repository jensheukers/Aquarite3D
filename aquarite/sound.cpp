/**
*	Filename: sound.cpp
*
*	Description: Source file for Sound class.
*
*	Version: 13/1/2019
*
*	© 2019, Jens Heukers
*/
#include "sound.h"
#include "debug.h"

Sound::Sound() {
	this->source = nullptr;
	this->pitch = 1;
	this->gain = 1;
}

void Sound::LoadAudioSource(std::string path) {
	Debug::Log("Loading audio source : " + path, typeid(*this).name());
	
	ALCenum error;

	AudioSource* aSource = new AudioSource(); // Create new audio source struct instance
	aSource->sourcePath = path; // Set path
	alGenSources((ALuint)1, &aSource->source); // Generate sources

	error = alGetError();
	if (error != AL_NO_ERROR) { // If error
		Debug::Log("Could not load audio source : " + error, typeid(*this).name());
		return;
	}

	//Generate buffers
	alGenBuffers((ALuint)1, &aSource->buffer);

	error = alGetError();
	if (error != AL_NO_ERROR) { // If error
		Debug::Log("Could not load audio source : " + error, typeid(*this).name());
		return;
	}
	
	this->source = aSource; // Set the source
}

void Sound::SetPitch(float pitch) {
	if (this->source == nullptr) return;

	alSourcef(this->source->source, AL_PITCH, (ALfloat)pitch);
	this->pitch = (ALfloat)pitch;
}

ALfloat Sound::GetPitch() {
	return this->pitch;
}

void Sound::SetGain(float gain) {
	if (this->source == nullptr) return;

	alSourcef(this->source->source, AL_GAIN, (ALfloat)gain);
	this->gain = (ALfloat)gain;
}

ALfloat Sound::GetGain() {
	return this->gain;
}

void Sound::SetPosition(Vec3 pos) {
	if (this->source == nullptr) return;

	alSource3f(this->source->source, AL_POSITION, (ALfloat)pos.x, (ALfloat)pos.y, (ALfloat)pos.z);
	this->position = pos;
}

Vec3 Sound::GetPosition() {
	return this->position;
}

void Sound::SetVelocity(Vec3 vel) {
	if (this->source == nullptr) return;

	alSource3f(this->source->source, AL_VELOCITY, (ALfloat)vel.x, (ALfloat)vel.y, (ALfloat)vel.z);
	this->velocity = velocity;
}

Vec3 Sound::GetVelocity() {
	return this->velocity;
}

void Sound::Loop(bool state) {
	if (this->source == nullptr) return;

	ALboolean s;
	if (state)
		s = true;
	else
		s = false;
	alSourcei(this->source->source, AL_LOOPING, s);
}