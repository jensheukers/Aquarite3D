// This file handles all communication between c++ and lua
//
//	Version: 7/2/2019
//
//	© 2019, Jens Heukers
#ifndef LUASCRIPT_H
#define LUASCRIPT_H
#include <string>
#include <vector>
#include "lua.hpp"

/**
* LuaScript is a singleton class, it can be called from anywhere in the program, it acts mostly as a "handler"
*/
class LuaScript {
private:
	static LuaScript* instance; /**< The singleton instance. */
	lua_State* state; /**< The global lua state. */

	/**
	* Gets the instance, if instance is nullptr creates a new instance
	* @return LuaScript*, pointer to the instance
	*/
	static LuaScript* GetInstance();

	/**
	* Constructor
	*/
	LuaScript();
public:
	/**
	* Runs a script to lua.
	* @param script, The script to execute
	* @return int, Returns int for error checking
	*/
	static int Run(std::string script);

	/**
	* Runs a function in lua.
	* @param file, The script to execute
	* @param function, The function to execute
	* @return std::string, value as a string type
	*/
	static std::string RunFunction(std::string file , std::string function, std::vector<std::string> arguments);

	/**
	* Adds a native C Function to lua stack.
	* @param name, The name of the function
	* @param function, The pointer to the function, Function should return string to lua, and have the state as parameter
	*/
	static void AddNativeFunction(std::string name, int(*func_pointer)(lua_State*));

	/**
	* Determines type of variable then returns type as a int
	* @param variableName, The name of the variable
	* @return int, variable Type enum
	*/
	static int GetType(std::string variableName);

	/**
	* Gets the global variable specified as parameter, pushes to the stack then returns number if checks are passed.
	* @param variableName, The name of the variable
	* @return int, The value returned by lua.
	*/
	static int GetNumber(std::string variableName);

	/**
	* Gets the global variable specified as parameter, pushes to the stack then returns string if checks are passed.
	* @param variableName, The name of the variable
	* @return std::string, The value returned by lua.
	*/
	static std::string GetString(std::string variableName);

	/**
	* Destructor
	*/
	~LuaScript();
};

#endif // !LUASCRIPT_H