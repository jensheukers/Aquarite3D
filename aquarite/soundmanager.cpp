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

	SoundManager::GetInstance()->listener = new Listener(); // Create default listener instance
	SoundManager::GetInstance()->listener->head = Vec3(0.0f, 0.0f, 1.0f);
	SoundManager::GetInstance()->listener->up = Vec3(0.0f, -1.0f, 0.0f);
	Debug::Log("Initialized", typeid(*_instance).name());
}

void SoundManager::Update(Vec3 head, Vec3 up) {
	Listener* listener = SoundManager::GetInstance()->listener;
	if (listener == nullptr) return;
	ALfloat _listenerOrientation[]{ listener->head.x, listener->head.y, listener->head.z, 
									listener->up.x, listener->up.y, listener->up.z };
	alListener3f(AL_POSITION, listener->position.x, listener->position.y, listener->position.z);
	alListener3f(AL_VELOCITY, listener->velocity.x, listener->velocity.y, listener->velocity.z);
	alListenerfv(AL_ORIENTATION, _listenerOrientation);

	//Play sounds
	for (size_t i = 0; i < SoundManager::GetInstance()->_sounds.size(); i++) {
		if (SoundManager::GetInstance()->_sounds[i]->GetAudioSource()->state != AL_STOPPED) {
			alGetSourcei(SoundManager::GetInstance()->_sounds[i]->GetAudioSource()->sourceID, 
				AL_SOURCE_STATE, &SoundManager::GetInstance()->_sounds[i]->GetAudioSource()->state);
		}
	}

}

void SoundManager::SetListenerPosition(Vec3 pos) {
	SoundManager::GetInstance()->listener->position = pos;
}

Vec3& SoundManager::GetListenerPosition() {
	return SoundManager::GetInstance()->listener->position;
}

void SoundManager::AddSound(Sound* sound) {
	SoundManager::GetInstance()->_sounds.push_back(sound);
}

Sound* SoundManager::GetSound(int index) {
	return SoundManager::GetInstance()->_sounds[index];
}

void SoundManager::RemoveSound(int index) {
	SoundManager::GetInstance()->_sounds.erase(SoundManager::GetInstance()->_sounds.begin() + index);
}

void SoundManager::PlaySound(int index) {
	alSourcePlay(SoundManager::GetSound(index)->GetAudioSource()->sourceID);
}

void SoundManager::PlaySound(Sound* sound) {
	for (size_t i = 0; i < SoundManager::GetInstance()->_sounds.size(); i++) {
		if (SoundManager::GetInstance()->_sounds[i] == sound) {
			SoundManager::PlaySound(i);
		}
	}
}

void SoundManager::StopSound(int index) {
	alSourceStop(SoundManager::GetSound(index)->GetAudioSource()->sourceID);
}

void SoundManager::StopSound(Sound* sound) {
	for (size_t i = 0; i < SoundManager::GetInstance()->_sounds.size(); i++) {
		if (SoundManager::GetInstance()->_sounds[i] == sound) {
			SoundManager::StopSound(i);
		}
	}
}

void SoundManager::Destroy() {
	alcMakeContextCurrent(NULL);
	alcDestroyContext(SoundManager::GetInstance()->context);
	alcCloseDevice(SoundManager::GetInstance()->device);

	//Remove and DELETE all sounds
	for (size_t i = 0; i < SoundManager::GetInstance()->_sounds.size(); i++) {
		delete SoundManager::GetInstance()->_sounds[i]; // Delete instance
		SoundManager::RemoveSound(i); // Remove pointer

		Debug::Log("Removed sound instance: " + i, typeid(*SoundManager::GetInstance()).name());
	}
}