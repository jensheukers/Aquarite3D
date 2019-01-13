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

class Sound {
private:
	ALuint buffer; /// @Brief The sound buffer object
	ALuint source; /// @Brief The source pointer to the buffer object
	ALuint state; /// @Brief The current source state
	std::string sourcePath; /// @brief The path to the source
public:
	/**
	* Loads a sound file into the buffer
	*/
	void LoadIntoBuffer(std::string path);
};


#endif // !SOUND_H