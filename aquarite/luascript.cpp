// This file handles all communication between c++ and lua
//
//	Version: 7/2/2019
//
//	© 2019, Jens Heukers
#include "luascript.h"
#include "debug.h"
#include "core.h"

LuaScript* LuaScript::instance; // Pointer to instance

LuaScript* LuaScript::GetInstance() {
	if (!instance) {
		instance = new LuaScript(); // Create instance
	}
	return instance;
}

LuaScript::LuaScript() {
	this->state = luaL_newstate(); // Create a new lua state
}

int LuaScript::Run(std::string script) {
	return luaL_dostring(LuaScript::GetInstance()->state, script.c_str());
}

std::string LuaScript::RunFunction(std::string file, std::string function, std::vector<std::string> arguments) {
	std::string absolutePath = Core::GetBuildDirectory();
	absolutePath.append(file);

	if (luaL_dofile(LuaScript::GetInstance()->state, absolutePath.c_str()) != LUA_OK) {
		return "Lua: Error opening file";
	}

	lua_getglobal(LuaScript::GetInstance()->state, function.c_str());

	if (lua_isfunction(LuaScript::GetInstance()->state, -1)) {
		for (size_t i = 0; i < arguments.size(); i++) {
			lua_pushstring(LuaScript::GetInstance()->state, arguments[i].c_str());
		}
		size_t num_args = arguments.size();
		lua_pcall(LuaScript::GetInstance()->state, num_args, 1, 0); // We expect 1 return
		return lua_tostring(LuaScript::GetInstance()->state, -1);
	}
	else {
		return "Lua: " + function + " Is not a function";
	}
}

void LuaScript::AddNativeFunction(std::string name, int(*func_pointer)(lua_State*)) {
	lua_pushcfunction(LuaScript::GetInstance()->state, func_pointer);
	lua_setglobal(LuaScript::GetInstance()->state, name.c_str());
}

int LuaScript::GetType(std::string variableName) {
	//Push to top of stack
	lua_getglobal(LuaScript::GetInstance()->state, variableName.c_str());

	//Return type
	return lua_type(LuaScript::GetInstance()->state, -1);
}

int LuaScript::GetNumber(std::string variableName) {
	//Push to top of stack
	lua_getglobal(LuaScript::GetInstance()->state, variableName.c_str());
		
	//Determine type of last pushed value
	int type = lua_type(LuaScript::GetInstance()->state, -1);
	
	if (type == LUA_TNUMBER) {
		return (int)lua_tonumber(LuaScript::GetInstance()->state, -1);
	}
	else {
		Debug::LogScreen("Lua: Variable is not a number!");
		return 0;
	}
}

std::string LuaScript::GetString(std::string variableName) {
	//Push to top of stack
	lua_getglobal(LuaScript::GetInstance()->state, variableName.c_str());

	//Determine type of last pushed value
	int type = lua_type(LuaScript::GetInstance()->state, -1);

	if (type == LUA_TSTRING) {
		return lua_tostring(LuaScript::GetInstance()->state, -1);
	}
	else {
		Debug::LogScreen("Lua: Variable is not a string!");
		return 0;
	}
}

LuaScript::~LuaScript() {
	lua_close(this->state); // Destroy the lua state
}