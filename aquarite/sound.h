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
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>
#include "math/vec3.h"

//AudioSource struct
struct AudioSource {
	std::string sourcePath; /// @brief The path to the source
	ALint state; /// @brief The state of the source
	ALuint bufferID; /// @brief Pointer to the sound buffer object
	ALuint sourceID; /// @brief The source pointer to the buffer object
	ALenum format; /// @brief the sound data format
	ALsizei freq; /// @brief The frequency of the sound data
	std::vector<char> bufferData; /// @brief Teh sound buffer data from file
};

class Sound {
private:
	ALuint state; /// @brief The current source state
	AudioSource* source; /// @brief the current audio source
	ALfloat pitch; ///@brief The pitch of the sound
	ALfloat gain; ///@brief The gain of the sound
	Vec3 position; ///@brief The position of the sound
	Vec3 velocity; ///@brief The velocity of the sound

	/**
	* Load a ogg file, if success return 1
	*/
	bool LoadOgg(char* fileName, std::vector<char> &buffer, ALenum& format, ALsizei &freq);
public:
	/**
	* Constructor
	*/
	Sound();

	/**
	* Destructor
	*/
	~Sound();

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

	/**
	* Returns the audio source, if not yet loaded returns nullptr
	*/
	AudioSource* GetAudioSource();
};


#endif // !SOUND_H