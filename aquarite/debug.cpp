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
#include "ui/text.h"
#include "core.h"

const char* Debug::PREFIX = "Aquarite"; // Set Prefix

Debug* Debug::instance; // Instance

void Debug::Initialize(Renderer* renderer) {
	Debug::GetInstance()->rendererInstance = renderer;
	Debug::GetInstance()->text = new Text();
	Debug::GetInstance()->color = Point4f(1.0f,0.5f,0.5f,1.0f);
}

Debug* Debug::GetInstance() {
	if (!instance) {
		instance = new Debug();
	}
	return instance;
}

void Debug::Log(std::string string, std::string callerName) {
	std::cout << Debug::PREFIX << " : " << "~" << callerName.c_str() << "~ " << string.c_str() << std::endl; // Log to console
}

void Debug::LogScreen(std::string string, std::string callerName) {
	Debug::GetInstance()->text->position = Vec3(0, Core::GetResolution().y);

	//Construct string
	std::string content = Debug::PREFIX;
	content.append(" : ");
	content.append("~");
	content.append(callerName);
	content.append("~ ");
	content.append(string);

	Debug::GetInstance()->text->SetColor(Debug::GetInstance()->color);
	Debug::GetInstance()->text->GetText().insert(0, content);
	Debug::GetInstance()->text->GetText().insert(content.size(),"\n");

	//Make sure we delete some text if string becomes too long (2150 chars), keep hardcoded for now
	if (Debug::GetInstance()->text->GetText().size() > 2150) {
		Debug::GetInstance()->text->GetText().erase(2150, Debug::GetInstance()->text->GetText().size());
	}
}

void Debug::NewFrame() {
	//Register to renderer
	Debug::GetInstance()->rendererInstance->RegisterText(Debug::GetInstance()->text);
}

void Debug::Clear() {
	Debug::GetInstance()->text->SetText("");
}

void Debug::SetColor(Point4f color) {
	Debug::GetInstance()->color = color;
}