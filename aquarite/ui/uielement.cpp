//	Filename: uielement.cpp
//
//	Description: Source file for UI element class
//
//	Version: 22/1/2019
//
//	© 2019, Jens Heukers
#include "uielement.h"
#include "../core.h"

void UIElement::Update() {

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

void UIElement::OnEnter() {

}

void UIElement::OnStay() {

}

void UIElement::OnLeave() {

}