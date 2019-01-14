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
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include "math/vec3.h"
#include "sound.h"

//Listener Structure
struct Listener {
	Vec3 position; /// @brief The listener's Position
	Vec3 velocity; /// @brief The listener's Velocity
	Vec3 head; /// @brief The orientation head
	Vec3 up; /// @brief The orientation head
};

class SoundManager {
private:
	static SoundManager* _instance; /// @brief The SoundManager instance

	//Local members
	ALCdevice* device; /// @brief The currently used audio device
	ALCcontext* context; /// @brief The currently active context
	Listener* listener; /// @brief The currently active listener instance

	std::vector<Sound*> _sounds; /// @brief Vector containing registered sounds

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
	static void Update(Vec3 position, Vec3 head, Vec3 up);

	/**
	* Adds a new sound to the sounds list
	*/
	static void AddSound(Sound* sound);

	/**
	* Returns a sound pointer, where index matches
	*/
	static Sound* GetSound(int index);

	/**
	* Removes a sound from the sounds list
	*/
	static void RemoveSound(int index);

	/**
	* Plays the sound
	*/
	static void PlaySound(int index);

	//Overload

	/**
	* Plays the sound
	*/
	static void PlaySound(Sound* sound);

	/**
	* Stops playing the sound
	*/
	static void StopSound(int index);

	//Overload

	/**
	* Stops playing the sound
	*/
	static void StopSound(Sound* sound);

	/**
	* Destroys the sound manager
	*/
	static void Destroy();
};


#endif // !SOUNDMANAGER_H
