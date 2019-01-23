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

//Include entity.h, texture.h
#include "../entity.h"
#include "../texture.h"

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
	void Render(Renderer* renderer, Camera* camera) override;
public:
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

#endif // !UIELEMENT_H