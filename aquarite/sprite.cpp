/**
*	Filename: sprite.cpp
*
*	Description: Source file for Sprite class.
*
*	Version: 17/1/2019
*
*	© 2018, Jens Heukers
*/
#include "sprite.h"

void Sprite::Render(Renderer* renderer, Camera* camera) {
	if (this->texture) {
		renderer->RegisterSprite(this); // Register the sprite
	}
}

Texture* Sprite::GetTexture() {
	return this->texture;
}

void Sprite::SetTexture(Texture* texture) {
	this->texture = texture;
}