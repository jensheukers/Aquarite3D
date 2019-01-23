//	Filename: uielement.cpp
//
//	Description: Source file for UI element class
//
//	Version: 23/1/2019
//
//	© 2019, Jens Heukers
#include "../debug.h"
#include "uielement.h"
#include "../core.h"
#include "../input.h"

UIElement::UIElement() {
	this->image = nullptr;
	this->mouseInBounds = false;
	this->mouseInBoundsLastFrame = false;
}

void UIElement::Update() {
	//First check if cursor is active, if not we dont want to check
	if (!Core::CursorEnabled()) return;

	//Check mouse positions
	if (PointInBounds(Input::GetMousePosition())) {
		if (mouseInBoundsLastFrame) {
			OnStay();
			mouseInBounds = true;
		}
		else {
			OnEnter();
		}

		mouseInBoundsLastFrame = true;
	}
	else {
		if (mouseInBoundsLastFrame) {
			OnLeave();
			mouseInBounds = false;
		}

		mouseInBoundsLastFrame = false;
	}
}

void UIElement::Render(Renderer* renderer, Camera* camera) {
	renderer->RegisterUIElement(this);
}

void UIElement::SetImage(Texture* image) {
	this->image = image;
}

Texture* UIElement::GetImage() {
	return this->image;
}

bool UIElement::PointInBounds(Point2f point) {
	if (this->image == nullptr) return false;

	//Check x component
	if (point.x < position.x || point.x > position.x + this->image->textureData->width)
		return false;

	//Check y component
	if (point.y < position.y || point.y > position.y + this->image->textureData->height)
		return false;

	//If cheks are passed return true
	return true;
}

void UIButton::Update() {
	//First check if cursor is active, if not we dont want to check
	if (!Core::CursorEnabled()) return;

	//Check mouse positions
	if (PointInBounds(Input::GetMousePosition())) {
		if (Input::GetButtonDown(BUTTONCODE_LEFT)) {
			OnClick(BUTTONCODE_LEFT);
		}

		if (Input::GetButtonDown(BUTTONCODE_MIDDLE)) {
			OnClick(BUTTONCODE_MIDDLE);
		}

		if (Input::GetButtonDown(BUTTONCODE_RIGHT)) {
			OnClick(BUTTONCODE_RIGHT);
		}

		if (mouseInBoundsLastFrame) {
			OnStay();
			mouseInBounds = true;
		}
		else {
			OnEnter();
		}

		mouseInBoundsLastFrame = true;
	}
	else {
		if (mouseInBoundsLastFrame) {
			OnLeave();
			mouseInBounds = false;
		}

		mouseInBoundsLastFrame = false;
	}
}

UIInputField::UIInputField() {
	this->text = nullptr;
	this->typing = true;
}

void UIInputField::Update() {
	//First check if cursor is active, if not we dont want to check
	if (this->image == nullptr) return;

	if (this->text != nullptr) {
		this->text->position = Vec3(this->position.x, this->position.y + this->GetImage()->textureData->height);
	}

	if (!Core::CursorEnabled()) return;

	if (PointInBounds(Input::GetMousePosition())) {
		if (Input::GetButtonDown(BUTTONCODE_LEFT)) {
			if (typing) {
				typing = false;
			}
			else {
				typing = true;
			}
		}

		if (mouseInBoundsLastFrame) {
			OnStay();
			mouseInBounds = true;
		}
		else {
			OnEnter();
		}

		mouseInBoundsLastFrame = true;
	}
	else {
		if (mouseInBoundsLastFrame) {
			OnLeave();
			mouseInBounds = false;
		}

		mouseInBoundsLastFrame = false;
	}

	if (typing) {
		if (Input::GetLastKey() == KEYCODE_EMPTY_KEY) return;

		//We translate the key press to character
		if (Input::GetLastKey() != KEYCODE_BACKSPACE) {
			text->GetText().append((const char*)Input::GetLastKey());
		}
		else {
			text->GetText().erase(text->GetText().begin() + text->GetText().length() - 1);
		}
	}
}

void UIInputField::SetTextInstance(Text* text) {
	if (this->GetParent() == nullptr) {
		Debug::Log("Error: please set the text instance after childing.", typeid(*this).name());
		return;
	}

	if (this->text != nullptr) {
		GetParent()->RemoveChild(text);
	}

	this->text = text;
	GetParent()->AddChild(text);
}