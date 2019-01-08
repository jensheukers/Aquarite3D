/**
*	Filename: debug.h
*
*	Description: Debug class for well debugging ;)
*
*	Version: 22/11/2018
*
*	© 2018, Jens Heukers
*/
#ifndef DEBUG_H
#define DEBUG_H
#include <iostream>

class Debug {
private:
	static const char* PREFIX; /// @brief Prefix, name to be used for every log
public:
	/**
	* Log string to the console
	*/
	static void Log(std::string string, std::string callerName);
};

#endif // !DEBUG_H

