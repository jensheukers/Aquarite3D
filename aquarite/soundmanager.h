/**
*	Filename: soundmanager.h
*
*	Description: Header file for SoundManager class.
*
*	Version: 13/1/2019
*
*	© 2019, Jens Heukers
*/
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <AL/al.h>
#include <AL/alc.h>

class SoundManager {
private:
	static SoundManager* _instance; /// @brief The SoundManager instance

	//Local members
	ALCdevice* device; /// @brief The currently used audio device

	/**
	* Returns the instance or creates one if not exists
	*/
	static SoundManager* GetInstance();

public:
	/**
	* Initializes the SoundManager
	*/
	static void Init();

	/**
	* Destroys the sound manager
	*/
	static void Destroy();
};


#endif // !SOUNDMANAGER_H
