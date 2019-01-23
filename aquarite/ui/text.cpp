//	Filename: text.cpp
//
//	Description: Source file for Text class.
//
//	Version: 23/1/2019
//
//	© 2019, Jens Heukers
#include "text.h"

void Text::Render(Renderer* renderer, Camera* camera) {
	renderer->RegisterText(this);
}

void Text::SetColor(Point4f color) {
	this->color = color;
}

Point4f Text::GetColor() {
	return this->color;
}

void Text::SetTextScale(float scale) {
	this->textScale = scale;
}

float Text::GetTextScale() {
	return this->textScale;
}

void Text::SetText(std::string text) {
	this->text = text;
}

std::string Text::GetText() {
	return this->text;
}