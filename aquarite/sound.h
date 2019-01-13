/**
*	Filename: sound.h
*
*	Description: Header file for Sound class.
*
*	Version: 13/1/2019
*
*	© 2019, Jens Heukers
*/
#ifndef SOUND_H
#define SOUND_H
#include <string>
#include <AL/al.h>
#include <AL/alc.h>
#include "math/vec3.h"

//AudioSource struct
struct AudioSource {
	ALuint buffer; /// @brief Pointer to the sound buffer object
	ALuint source; /// @brief The source pointer to the buffer object
	std::string sourcePath; /// @brief The path to the source
};

class Sound {
private:
	ALuint state; /// @brief The current source state
	AudioSource* source; /// @brief the current audio source
	ALfloat pitch; ///@brief The pitch of the sound
	ALfloat gain; ///@brief The gain of the sound
	Vec3 position; ///@brief The position of the sound
	Vec3 velocity; ///@brief The velocity of the sound
public:
	/**
	* Constructor
	*/
	Sound();

	/**
	* Loads a AudioSource and sets source pointer to it
	*/
	void LoadAudioSource(std::string path);

	/**
	* Set the pitch of the sound
	*/
	void SetPitch(float pitch);

	/**
	* Returns the pitch of the sound
	*/
	ALfloat GetPitch();

	/**
	* Set the gain of the sound
	*/
	void SetGain(float gain);

	/**
	* Returns the gain of the sound
	*/
	ALfloat GetGain();

	/**
	* Sets the position of the sound
	*/
	void SetPosition(Vec3 pos);

	/**
	* Gets the position of the sound
	*/
	Vec3 GetPosition();

	/**
	* Sets the velocity of the sound
 	*/
	void SetVelocity(Vec3 vel);

	/**
	* Gets the velocity of the sound
	*/
	Vec3 GetVelocity();

	/**
	* If true sound will loop, else wont
	*/
	void Loop(bool state);
};


#endif // !SOUND_H