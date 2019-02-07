// This file handles all communication between c++ and lua
//
//	Version: 7/2/2019
//
//	© 2019, Jens Heukers
#include "luascript.h"
#include "debug.h"

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