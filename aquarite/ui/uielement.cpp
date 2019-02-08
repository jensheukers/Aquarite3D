//	Filename: uielement.cpp
//
//	Description: Source file for UI element class
//
//	Version: 6/2/2019
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

	for (size_t i = 0; i < GetChildren().size(); i++) {
		//Cast to our type to allow for render call
		UIElement* element = dynamic_cast<UIElement*>(GetChild(i));

		if (element != NULL) {
			element->Render(renderer, camera);
		}
	}
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
	if (point.x < GetPositionGlobal().x || point.x > GetPositionGlobal().x + this->image->textureData->width)
		return false;

	//Check y component
	if (point.y > GetPositionGlobal().y || point.y < GetPositionGlobal().y - this->image->textureData->height)
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

#define NON_ALLOWED_INPUTFIELD_CHARS_SIZE 9
int nonAllowedInputFieldChars[NON_ALLOWED_INPUTFIELD_CHARS_SIZE] = {
	KEYCODE_RIGHT_SHIFT,
	KEYCODE_LEFT_SHIFT,
	KEYCODE_ENTER,
	KEYCODE_ESCAPE,
	KEYCODE_GRAVE_ACCENT,
	KEYCODE_LEFT,
	KEYCODE_RIGHT,
	KEYCODE_UP,
	KEYCODE_DOWN
};

void UIInputField::Render(Renderer* renderer, Camera* camera) {
	renderer->RegisterUIElement(this);
	renderer->RegisterText(text);
}

UIInputField::UIInputField() {
	this->text = nullptr;
	this->typing = false;
}

void UIInputField::Update() {
	//First check if cursor is active, if not we dont want to check
	if (this->image == nullptr) return;

	if (this->text != nullptr) {
		this->text->position = Vec3(this->GetPositionGlobal().x, this->GetPositionGlobal().y + this->GetImage()->textureData->height);
	}

	if (!Core::CursorEnabled()) return;

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

	if (typing) {
		if (Input::GetLastKey() == KEYCODE_EMPTY_KEY) return;

		//We translate the key press to character
		if (Input::GetLastKey() != KEYCODE_BACKSPACE) {

			//Check for key combo's
			if (Input::GetKey(KEYCODE_RIGHT_SHIFT) || Input::GetKey(KEYCODE_LEFT_SHIFT)) {
				if (Input::GetKeyDown(KEYCODE_9)) {
					text->GetText().push_back('(');
				}
				else if (Input::GetKeyDown(KEYCODE_0)) {
					text->GetText().push_back(')');
				}
				else if (Input::GetKeyDown(KEYCODE_APOSTROPHE)) {
					text->GetText().push_back('"');
				}
				else if (Input::GetKeyDown(KEYCODE_EQUAL)) {
					text->GetText().push_back('+');
				}
				else {
					bool falseKeyFound = false;

					for (int i = 0; i < NON_ALLOWED_INPUTFIELD_CHARS_SIZE; i++) {
						if (nonAllowedInputFieldChars[i] == Input::GetLastKey()) {
							falseKeyFound = true;
						}
					}

					if (!falseKeyFound) {
						text->GetText().push_back((char)Input::GetLastKey());
					}
				}
			}
			else {
				bool falseKeyFound = false;

				for (int i = 0; i < NON_ALLOWED_INPUTFIELD_CHARS_SIZE; i++) {
					if (nonAllowedInputFieldChars[i] == Input::GetLastKey()) {
						falseKeyFound = true;
					}
				}

				if (!falseKeyFound) {
					text->GetText().push_back(tolower((char)Input::GetLastKey()));
				}
			}
		}
		else {
			if (text->GetText().length() > 0) {
				text->GetText().erase(text->GetText().begin() + text->GetText().length() - 1);
			}
		}
		lastTypeTime = Core::GetTimeElapsed();
		
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
	this->AddChild(text);
}

void UIInputField::AllowTyping(bool state) {
	typing = state;
}