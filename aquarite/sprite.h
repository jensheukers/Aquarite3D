/**
*	Filename: sprite.h
*
*	Description: Header file for Sprite class.
*
*	Version: 17/1/2019
*
*	© 2018, Jens Heukers
*/
#ifndef SPRITE_H
#define SPRITE_H
#include "texture.h"
#include "entity.h"

//Sprite inherits from entity but override's the Render() method.
class Sprite : public Entity {
private:
	Texture* texture; /// @brief The texture of the sprite
protected:
	/**
	* Overwritten Render() method.
	*/
	void Render(Renderer* renderer, Camera* camera) override;
public:
	/**
	* Returns the texture of the sprite
	*/
	Texture* GetTexture();

	/**
	* Sets the texture of the sprite
	*/
	void SetTexture(Texture* texture);
};

#endif // !SPRITE_H