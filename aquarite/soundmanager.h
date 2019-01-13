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
#include "math/vec3.h"

//Listener structure
struct Listener {
	Vec3 position; /// @brief The listener's Position
	Vec3 velocity; /// @brief The listener's Velocity
};

class SoundManager {
private:
	static SoundManager* _instance; /// @brief The SoundManager instance

	//Local members
	ALCdevice* device; /// @brief The currently used audio device
	ALCcontext* context; /// @brief The currently active context
	Listener* listener; /// @brief The currently active listener instance

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
	* Update gets called each frame by core, it handles things like listener's position ect.
	*/
	static void Update();

	/**
	* Sets the currently active listener
	*/
	static void SetListener(Listener* listener);

	/**
	* Returns the currently active listener
	*/
	static Listener* GetListener();

	/**
	* Destroys the sound manager
	*/
	static void Destroy();
};


#endif // !SOUNDMANAGER_H
