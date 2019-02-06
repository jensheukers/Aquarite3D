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
#include "math/pointx.h"

class Renderer;
class Text;

class Debug {
private:
	static const char* PREFIX; /// @brief Prefix, name to be used for every log
	static Debug* instance; /// @brief the instance of the debug class
	Renderer* rendererInstance; /// @brief the renderer instance.
	Text* text; /// @brief The text instance for drawing debug texts
	Point4f color; /// @brief the color of the log text
public:
	/**
	* Initializes the debug, only to be called by core
	*/
	static void Initialize(Renderer* renderer);

	/**
	* Returns the instance of the debug class, if it does not exist it will create a new instance
	*/
	static Debug* GetInstance();

	/**
	* Log string to the console
	*/
	static void Log(std::string string, std::string callerName);

	/**
	* Log string to the screen, and not to the console, avoiding the heavy cout call
	*/
	static void LogScreen(std::string string);

	/**
	* registers the text to the renderer
	*/
	static void NewFrame();

	/**
	* Clears the information from the previous frame.
	*/
	static void Clear();

	/**
	* Set the color
	*/
	static void SetColor(Point4f color);
};

#endif // !DEBUG_H

