//	Filename: uielement.h
//
//	Description: Header file for UI element class, UIElement extends from entity.
//				 This class also contains definitions for: UIButton.
//
//	Version: 23/1/2019
//
//	© 2019, Jens Heukers
#ifndef UIELEMENT_H
#define UIELEMENT_H

//Include entity.h, texture.h, text.h
#include "../entity.h"
#include "../texture.h"
#include "text.h"

/**
* UIElement extends from Entity class, and overrides the Render method
*/
class UIElement : public Entity {
protected:
	Texture* image; /**< Image to be displayed by the UIElement. */
	bool mouseInBounds; /**< True if mouse is inside bounds */
	bool mouseInBoundsLastFrame; /**< True if mouse is inside bounds last frame*/

	/*
	* Overridden Render method, Calls the renderer
	* @param renderer, The renderer instance
	* @param camera, The currently active camera
	* @return void
	*/
	virtual void Render(Renderer* renderer, Camera* camera) override;
public:
	/**
	* Constructor
	*/
	UIElement();

	/**
	* Overridden Update method.
	* @return void
	*/
	void Update() override;

	/**
	* Sets the image of the UIElement
	* @param image, The texture instance of the image
	* @return void
	*/
	void SetImage(Texture* image);

	/**
	* Returns the image as a texture instance
	* @return Texture pointer
	*/
	Texture* GetImage();

	/**
	* Virtual method, Is triggered when user enters the ui element with the mouse
	* @return void
	*/
	virtual void OnEnter() {};

	/**
	* Virtual method, Is triggered when user has mouse inside the element
	* @return void
	*/
	virtual void OnStay() {};

	/**
	* Virtual Method, Is triggered when user leaves the element
	*/
	virtual void OnLeave() {};

	/**
	* Returns true if point on screen is inside the UIElement's bounds
	*/
	bool PointInBounds(Point2f point);
};

/**
* UIButton extends from UIElement, but has more functionality
*/
class UIButton : public UIElement {
public:
	/**
	* Overridden Update method.
	* @return void
	*/
	void Update() override;

	/**
	* Virtual method, Is triggered when user clicks on the button.
	* @param The button clicked.
	* @return void
	*/
	virtual void OnClick(int btn) {};
};

/**
* UIInputfield extends from UIElement, but has a text element
*/
class UIInputField : public UIElement {
private:
	Text* text; /**< The text element of the input field. */
	bool typing; /**< True if user is actively typing on the input field*/
	unsigned int lastTypeTime; /**< The moment last key was typed**/
protected:
	void Render(Renderer* renderer, Camera* camera) override;
public:
	/**
	* Constructor
	*/
	UIInputField();

	/**
	* Overridden Update method.
	* @return void
	*/
	void Update() override;

	/**
	* Sets the text instance
	* @param text, The Text Instance
	* @return void
	*/
	void SetTextInstance(Text* text);

	/**
	* Sets the bool for typing, if true every key will be processed, Note that Cursor has to be enabled to type
	*/
	void AllowTyping(bool state);
};

#endif // !UIELEMENT_H