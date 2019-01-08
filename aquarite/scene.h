/**
*	Filename: scene.h
*
*	Description: Header file for Scene class, Scene extends from Entity
*
*	Version: 27/12/2018
*
*	© 2018, Jens Heukers
*/

#ifndef SCENE_H
#define SCENE_H
#include <string>
#include "entity.h"
#include "camera.h"

class Light; // Forward Declaration

class Scene : public Entity {
private:
	Camera* activeCamera; /// @ brief The currently active camera
public:
	/**
	* Constructor
	*/
	Scene();

	/**
	* Update method
	*/
	virtual void Update() override {};

	/**
	* Update all scene children
	*/
	void UpdateSceneChildren();

	/**
	* Renders all scene children
	*/
	void RenderSceneChildren(Renderer* renderer, Camera* camera);

	/**
	* Returns the currently active camera
	*/
	Camera* GetActiveCamera();

	/**
	* Sets the currently active camera
	*/
	void SetActiveCamera(Camera* camera);

	/**
	* Adds a light to the renderer's Light list
	*/
	void AddLight(Light* light);

	/**
	* Removes a light from the renderer's Light list
	*/
	void RemoveLight(Light* light);

	/**
	* Loads scene data from a .ascene file, and inserts the data, offset parameter is the path from the main build directory
	*/
	void LoadSceneData(std::string offset);
};

#endif // !SCENE_H
