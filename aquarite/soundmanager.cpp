/**
*	Filename: soundmanager.cpp
*
*	Description: Source file for SoundManager class.
*
*	Version: 13/1/2019
*
*	© 2019, Jens Heukers
*/
#include "soundmanager.h"
#include "debug.h"

SoundManager* SoundManager::_instance;

SoundManager* SoundManager::GetInstance() {
	if (!_instance) {
		_instance = new SoundManager();
	}
	return _instance;
}

void SoundManager::Init() {
	SoundManager::GetInstance()->listener = nullptr;
	SoundManager::GetInstance()->device = alcOpenDevice(NULL);

	if (!SoundManager::GetInstance()->device) {
		Debug::Log("Could not load audio device", typeid(*_instance).name());
	}

	SoundManager::GetInstance()->context = alcCreateContext(SoundManager::GetInstance()->device, NULL);
	if (!alcMakeContextCurrent(SoundManager::GetInstance()->context)) {
		Debug::Log("Could not set current audio context", typeid(*_instance).name());
	}

	Debug::Log("Initialized", typeid(*_instance).name());
}

void SoundManager::Update() {
	Listener* listener = SoundManager::GetListener();
	if (listener == nullptr) return;

	ALfloat _listenerOrientation[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	alListener3f(AL_POSITION, listener->position.x, listener->position.y, listener->position.z);
	alListener3f(AL_VELOCITY, listener->velocity.x, listener->velocity.y, listener->velocity.z);
	alListenerfv(AL_ORIENTATION, _listenerOrientation);
}

void SoundManager::SetListener(Listener* listener) {
	SoundManager::GetInstance()->listener = listener;
}

Listener* SoundManager::GetListener() {
	return SoundManager::GetInstance()->listener;
}

void SoundManager::Destroy() {

}