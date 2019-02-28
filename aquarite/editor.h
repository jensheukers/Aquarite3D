//  Main header file for editor class, Editor is a singleton class
//
//	Version: 15/1/2019
//
//	© 2019, Jens Heukers

#ifndef EDITOR_H
#define EDITOR_H
#include "../external/imgui/imgui.h"
#include "camera.h"

class Editor {
private:
	static Editor* instance; /**< The instance to the editor*/
	bool active; /**< Determines if editor is active or not*/
	FPSCamera* camera; /**< The camera of the editor*/
	Camera* activeCamera; /**< The camera active before user went into editor*/

	/**
	* Returns the instance of the editor, or creates a new instance if it does not exist
	* @return Editor pointer
	*/
	static Editor* GetInstance();
public:
	/**
	* Initializes the editor, sets up style ect.
	*/
	static void Initialize();

	/**
	* Gets called by core each frame, handles updates to ImGUI windows
	*/
	static void Update();

	/**
	* Returns true if the editor is active else returns false
	* @return bool
	*/
	static bool Active();

	/**
	* Sets the active state to parameter
	* @param bool
	*/
	static void SetActive(bool state);

	/**
	* Returns the camera of the editor
	* @return camera pointer
	*/
	static Camera* GetCamera();

	/**
	* Returns the camera that was used before user went into editor
	* @return camera pointer
	*/
	static Camera* GetActiveCamera();
};

#endif // !EDITOR_H
