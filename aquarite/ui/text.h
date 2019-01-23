//	Filename: text.h
//
//	Description: Header file for Text class.
//
//	Version: 23/1/2019
//
//	© 2019, Jens Heukers
#ifndef TEXT_H
#define TEXT_H
#include <iostream>
#include "../entity.h"
#include "../math/pointx.h"

/**
* Text class extends from Entity
*/
class Text : public Entity {
private:
	Point4f color; /**< The color of the text */
	float textScale; /**< The scale of the text*/
	std::string text; /**< The contents of the text */

protected:
	/*
	* Overridden Render method, Calls the renderer
	* @param renderer, The renderer instance
	* @param camera, The currently active camera
	* @return void
	*/
	void Render(Renderer* renderer, Camera* camera) override;
public:
	/**
	* Constructor
	*/
	Text();

	/**
	* Sets the color of the Text
	* @param color, Point4f instance defining r,g,b,a values
	* @return void
	*/
	void SetColor(Point4f color);

	/**
	* Returns the color
	* @return Point4f
	*/
	Point4f GetColor();

	/**
	* Sets the scale of the Text
	* @param scale, float defining scale
	* @return void
	*/
	void SetTextScale(float scale);

	/**
	* Returns the scale
	* @return float
	*/
	float GetTextScale();

	/**
	* Sets the Text
	* @param text, string defining the text
	* @return void
	*/
	void SetText(std::string text);

	/**
	* Returns the text
	* @return std::string
	*/
	std::string GetText();
};

#endif // !TEXT_H
