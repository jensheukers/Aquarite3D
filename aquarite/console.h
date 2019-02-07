// Console class header file for Aquarite3D, the console will be available if debug mode is on,
// In the console class user will be able to execute lua script
// The console class is a singleton instance, meaning it can be called from everywhere
//
//	Version: 7/2/2019
//
//	© 2019, Jens Heukers
#ifndef CONSOLE_H
#define CONSOLE_H
#include <iostream>
#include "ui/uielement.h"

class Renderer; // Forward declaration

class Console {
private:
	//Pointers
	static Console* instance; /**< The console instance*/
	UIElement* console; /**< The console entity*/
	UIInputField* inputField; /**< The inputfield entity */
	Text* inputText; /**< Pointer to the current input*/
	Text* logText; /**< The text displayed in the console*/

	//List / maps
	std::vector<std::string> logTextLines; /**< list of lines in the log text*/
	std::vector<std::string> history; /**< History of executed commands*/
	std::map<std::string, std::string(*)(std::string)> commands; /**< List of commands, holding as function pointers as value*/

	//Ints
	int historyIndex; /**< Index of the currently browsing history*/

	//Booleans
	bool showConsole; /**< Defines if the console should be shown or not*/
	bool lastCursorState; /**< Remembers the last cursor state*/

	/**
	* Returns the console instance, if not found will create new instance
	* @return the console instance
	*/
	static Console* GetInstance();
public:
	/**
	* Initializes the console
	* @return void
	*/
	static void Initialize();

	/**
	* Updates the console and its children
	*/
	static void Update();

	/**
	* Handles rendering for the console, since we do not want the console to be a child of any scene, but a global entity
	* @param Renderer, the renderer instance
	*/
	static void Render(Renderer* renderer);

	/**
	* Adds a command to the command's list.
	* @param std::string, Name of the command
	* @param std::string(*)(std::string), Function pointer to method to run when called
	*/
	static void AddCommand(std::string name, std::string(*callback_func)(std::string));

	/**
	* Prints a string to the console
	* @param std::string, the string to print
	*/
	static void Log(std::string value);
};

#endif // !CONSOLE_H