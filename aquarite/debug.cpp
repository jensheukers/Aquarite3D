/**
*	Filename: debug.cpp
*
*	Description: Source files for Debug class
*
*	Version: 22/11/2018
*
*	© 2018, Jens Heukers
*/
#include "debug.h"

const char* Debug::PREFIX = "Aquarite"; // Set Prefix

void Debug::Log(std::string string, std::string callerName) {
	std::cout << Debug::PREFIX << " : " << "~" << callerName.c_str() << "~ " << string.c_str() << std::endl; // Log to console
}