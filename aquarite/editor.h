//  Main header file for editor class, Editor is a singleton class
//
//	Version: 4/3/2019
//
//	© 2019, Jens Heukers

#ifndef EDITOR_H
#define EDITOR_H
#include "../external/imgui/imgui.h"
#include "camera.h"
#include "entity.h"

class Editor {
private:
	static Editor* instance; /**< The instance to the editor*/
	bool active; /**< Determines if editor is active or not*/
	FPSCamera* camera; /**< The camera of the editor*/
	Camera* activeCamera; /**< The camera active before user went into editor*/
	Entity* currentSelection; /**< The currently selected entity*/


	//Submenu's
	bool loadMenuActive; /**< If true load menu will be rendered*/
	bool saveMenuActive; /**< If true save menu will be rendered*/
	bool entityListActive; /**< If true entity list menu will be rendered*/
	bool entityInfoActive; /**< If true entity info menu will be rendered*/
	bool scriptConsoleActive; /**< If true script menu will be rendered*/

	/**
	* Returns the instance of the editor, or creates a new instance if it does not exist
	* @return Editor pointer
	*/
	static Editor* GetInstance();

	/**
	* Handles the scene loading menu, should be called by update every frame, whenever active
	*/
	void HandleLoadMenu();

	/**
	* Handles the scene loading menu, should be called by update every frame, whenever active
	*/
	void HandleSaveMenu();

	/**
	* Handles the entity list menu, should be called by update every frame, whenever active
	*/
	void HandleEntityListMenu();

	/**
	* Handles the entity info menu, should be called by update every frame, whenever active
	*/
	void HandleEntityInfoMenu();

	/**
	* Handles the script menu, should be called by update every frame, whenever active
	*/
	void HandleScriptMenu();

	/**
	* Adds a point light to the scene
	*/
	void AddPointLight();

	/**
	* Adds a directional light to the scene
	*/
	void AddDirectionalLight();

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
