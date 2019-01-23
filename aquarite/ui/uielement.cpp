//	Filename: uielement.cpp
//
//	Description: Source file for UI element class
//
//	Version: 23/1/2019
//
//	© 2019, Jens Heukers
#include "uielement.h"
#include "../core.h"
#include "../input.h"

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