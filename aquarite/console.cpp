// Console class source code for Aquarite3D, the console will be available if debug mode is on,
// In the console class user will be able to execute lua script
//
//	Version: 7/2/2019
//
//	© 2019, Jens Heukers
#include <sstream>
#include "console.h"
#include "core.h"
#include "scenemanager.h"
#include "renderer.h"
#include "input.h"
#include "luascript.h"

//Default Console Commands
//Set a value in lua
std::string Set(std::string value) {
	int error = LuaScript::Run(value);
	return "Lua: " + value;
}

//Get a value from lua
std::string Get(std::string value) {
	int type = LuaScript::GetType(value);

	switch (type) {
		case LUA_TNUMBER:
			return "Lua: " + std::to_string(LuaScript::GetNumber(value));
			break;
		case LUA_TSTRING:
			return "Lua: " + LuaScript::GetString(value);
			break;
	}

	return "Lua: Cannot determine type";
}

Console* Console::instance; // Console singleton instance

Console* Console::GetInstance() {
	if (!instance) {
		instance = new Console();
	}

	return instance;
}

void Console::Initialize() {
	//BACKGROUND
	//Create bg texture for console
	Texture* bg = new Texture();
	bg->GenerateTexture(Core::GetResolution().x, Core::GetResolution().y / 2, GL_RGBA);
	bg->SetColor(Point4f(0.f, 0.f, 0.f, 35.f));

	Console::GetInstance()->console = new UIElement();
	Console::GetInstance()->console->SetImage(bg);
	Console::GetInstance()->console->SetPositionGlobal(Vec3(0, (float)Core::GetResolution().y / 2, 0));

	//Create textbox inside console
	Texture* textBoxBG = new Texture();
	textBoxBG->GenerateTexture(Core::GetResolution().x, 16, GL_RGBA);
	textBoxBG->SetColor(Point4f(0.f, 0.f, 0.f, 50.f));

	//Initialize inputfield
	Console::GetInstance()->inputField = new UIInputField();
	Console::GetInstance()->inputField->SetPositionGlobal(Console::GetInstance()->console->GetPositionGlobal());
	Console::GetInstance()->inputField->SetImage(textBoxBG);

	//Initialize text
	Console::GetInstance()->inputText = new Text();
	Console::GetInstance()->inputText->position = Console::GetInstance()->inputField->GetPositionGlobal() + Vec3(0, (float)textBoxBG->textureData->height, 0);

	//Child inputfield
	Console::GetInstance()->console->AddChild(Console::GetInstance()->inputField);
	Console::GetInstance()->inputField->AddChild(Console::GetInstance()->inputText);
	Console::GetInstance()->inputField->SetTextInstance(Console::GetInstance()->inputText);
	Console::GetInstance()->inputField->AllowTyping(true);

	//Log text
	Console::GetInstance()->logText = new Text();
	Console::GetInstance()->logText->position = Vec3(0, (float)Core::GetResolution().y, 0);
	Console::GetInstance()->console->AddChild(Console::GetInstance()->logText);

	Console::GetInstance()->showConsole = false;

	//Add default commands
	Console::AddCommand("set", Set);
	Console::AddCommand("get", Get);
}

void Console::Update() {
	if (Input::GetKeyUp(KEYCODE_GRAVE_ACCENT)) {
		if (!Console::GetInstance()->showConsole) {
			Console::GetInstance()->showConsole = true;
			Console::GetInstance()->lastCursorState = Core::CursorEnabled();
			Core::SetCursorEnabled(true); // Always enable cursor when console is enabled
		}
		else {
			Console::GetInstance()->showConsole = false;
			Core::SetCursorEnabled(Console::GetInstance()->lastCursorState);
		}
	}

	if (Console::GetInstance()->showConsole) {
		Console::GetInstance()->inputField->Update(); // Update input field
	}

	if (Input::GetKeyDown(KEYCODE_UP)) {
		if ((Console::GetInstance()->history.size() - 1) - Console::GetInstance()->historyIndex != -1) {
			Console::GetInstance()->inputText->SetText(Console::GetInstance()->history[(Console::GetInstance()->history.size() - 1) - Console::GetInstance()->historyIndex]);
			Console::GetInstance()->historyIndex++;
		}
	}

	if (Input::GetKeyDown(KEYCODE_ENTER)) { // If enter is pressed parse command
		//Add to history
		Console::GetInstance()->history.push_back(Console::GetInstance()->inputText->GetText());
		Console::GetInstance()->historyIndex = 0;

		//First split the input line string
		std::stringstream ss(Console::GetInstance()->inputText->GetText());
		std::string segment;
		std::vector<std::string> segments;
		while (std::getline(ss, segment, ' ')) { // Split by space character
			segments.push_back(segment);
		}

		std::string logString = "Unknown Command: " + segments[0];

		//Set command, and restruct value
		std::string command = segments[0];
		std::string value = "";

		for (size_t i = 1; i < segments.size(); i++) {
			if (i != segments.size() - 1)
				value.append(segments[i] + " "); // Since we split at space also re-add a space
			else
				value.append(segments[i]); // If last then just add segment
		}

		//Find command in commands list, then execute function pointer and set logscreen as result
		for (std::map<std::string, std::string(*)(std::string)>::reverse_iterator it = Console::GetInstance()->commands.rbegin(); it != Console::GetInstance()->commands.rend(); ++it) {
			if (it->first == command) { // If command found
				logString = it->second(value); // Call function pointer
			}
		}
		//Add line
		Console::GetInstance()->logTextLines.push_back(logString + "\n"); // add a line to list

		//Clear input field
		Console::GetInstance()->inputText->SetText("");
	}

	//Check if we dont exceed max log size of 20
	if (Console::GetInstance()->logTextLines.size() > 20) {
		Console::GetInstance()->logTextLines.erase(Console::GetInstance()->logTextLines.begin());
	}
}

void Console::Render(Renderer* renderer) {
	if (Console::GetInstance()->showConsole) {
		renderer->RegisterUIElement(Console::GetInstance()->console);
		renderer->RegisterUIElement(Console::GetInstance()->inputField);
		renderer->RegisterText(Console::GetInstance()->inputText);

		//Add all lines to string
		std::string finalString;
		for (size_t i = 0; i < Console::GetInstance()->logTextLines.size(); i++) {
			finalString.append(Console::GetInstance()->logTextLines[i]);
		}
		Console::GetInstance()->logText->SetText(finalString);

		renderer->RegisterText(Console::GetInstance()->logText);
	}
}

void Console::AddCommand(std::string name, std::string(*callback_func)(std::string)) {
	Console::GetInstance()->commands[name] = callback_func;
}

void Console::Log(std::string value) {
	Console::GetInstance()->logTextLines.push_back(value + "\n"); // add a line to list
}