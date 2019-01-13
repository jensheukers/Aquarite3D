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
	SoundManager::GetInstance()->device = alcOpenDevice(NULL);

	if (!SoundManager::GetInstance()->device) {
		Debug::Log("Could not load audio device", typeid(*_instance).name());
	}
}

void SoundManager::Destroy() {

}