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

void Sound::LoadIntoBuffer(std::string path) {
	/*Debug::Log("Loading new sound file: " + path, typeid(*this).name());
	
	// Load data into buffer
	buffer = alutCreateBufferFromFile(path.c_str());

	// Create sound source (use buffer to fill source)
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);

	if (alutGetError() == ALUT_ERROR_NO_ERROR) {
		Debug::Log("Succesfully loaded sound file", typeid(*this).name());
	}*/
}