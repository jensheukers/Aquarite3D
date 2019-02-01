/**
*	Filename: input.cpp
*
*	Description: Source file for Input class.
*
*	Version: 23/1/2019
*
*	© 2019, Jens Heukers
*/

#include "input.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (action) {
	case GLFW_PRESS: // On Key press
		Input::SetKey(key, true); // Set to true
		break;
	case GLFW_RELEASE: // On Key Release
		Input::SetKey(key, false); // Set to false
		break;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	switch (action) {
	case GLFW_PRESS: // On Key press
		Input::SetButton(button, true); // Set to true
		break;
	case GLFW_RELEASE: // On Key Release
		Input::SetButton(button, false); // Set to false
		break;
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Input::SetMousePos(Point2f((float)xpos, (float)ypos));
}

Input* Input::_instance; // Declare static member

Input* Input::GetInstance() {
	if (!_instance) {
		_instance = new Input();
		_instance->mousePicker = new MousePicker(); // Set mousepicker instance
	}
	
	return _instance;
}

void Input::HandleUpdates() {
	// Set Key Last
	for (size_t i = 0; i < Input::GetInstance()->_keys.size(); i++) {
		Input::GetInstance()->_keysLast[i] = Input::GetInstance()->_keys[i];
	}

	for (size_t i = 0; i < Input::GetInstance()->_buttons.size(); i++) {
		Input::GetInstance()->_buttonsLast[i] = Input::GetInstance()->_buttons[i];
	}

	Input::GetInstance()->lastKey = KEYCODE_EMPTY_KEY;
	Input::GetInstance()->mousePicker->Update(); // Update mouse picker
}

void Input::Init(GLFWwindow* window) {
	Input* instance = Input::GetInstance();
	glfwSetKeyCallback(window, key_callback); // Set key callback
	glfwSetMouseButtonCallback(window, mouse_button_callback); // Set button callback
	glfwSetCursorPosCallback(window, cursor_position_callback); // Set mouse pos callback
}

bool Input::GetKeyDown(int keyCode) {
	return GetInstance()->_keys[keyCode] && !GetInstance()->_keysLast[keyCode];
}

bool Input::GetKey(int keyCode) {
	return GetInstance()->_keys[keyCode] && GetInstance()->_keysLast[keyCode];
}

bool Input::GetKeyUp(int keyCode) {
	return !GetInstance()->_keys[keyCode] && GetInstance()->_keysLast[keyCode];
}

void Input::SetKey(int keyCode, bool state) {
	Input::GetInstance()->_keys[keyCode] = state;
	Input::GetInstance()->lastKey = keyCode;
}

bool Input::GetButtonDown(int buttonCode) {
	return GetInstance()->_buttons[buttonCode] && !GetInstance()->_buttonsLast[buttonCode];
}

bool Input::GetButton(int buttonCode) {
	return GetInstance()->_buttons[buttonCode] && GetInstance()->_buttonsLast[buttonCode];
}

bool Input::GetButtonUp(int buttonCode) {
	return !GetInstance()->_buttons[buttonCode] && GetInstance()->_buttonsLast[buttonCode];
}

void Input::SetButton(int buttonCode, bool state) {
	Input::GetInstance()->_buttons[buttonCode] = state;
}

void Input::SetMousePos(Point2f point) {
	Input::GetInstance()->_mousePos = point;
}

Point2f Input::GetMousePosition() {
	return Input::GetInstance()->_mousePos;
}

int Input::GetLastKey() {
	return Input::GetInstance()->lastKey;
}

Vec3 Input::GetMousePositionWorldSpace() {
	return Input::GetInstance()->mousePicker->GetCurrentRay();
}