/**
*	Filename: scenemanager.h
*
*	Description: Header file for SceneManager class, SceneManager is a singleton instance, 
*				 meaning it is avaiable everywhere on the program
*
*	Version: 23/11/2018
*
*	© 2018, Jens Heukers
*/
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <vector>
#include <string>
#include "scene.h"

class SceneManager {
private:
	//Global
	static SceneManager* _instance; /// @brief Global scenemanager instance variable

	//Local
	std::vector<Scene*> scenes; /// @brief Vector containing all scenes
	Scene* activeScene; /// @brief pointer to the currently active scene

	/**
	* Private Constructor
	*/
	SceneManager();
public:

	/**
	* Returns the singleton instance of the SceneManager, if instance pointer is nullptr. It will create a new instance
	*/
	static SceneManager* GetInstance();

	/**
	* Returns the currently active scene, if there is no active scene returns nullptr
	*/
	static Scene* GetActiveScene();

	/**
	* Sets the active scene to a scene from the scenes vector at given index, then returns pointer to that scene
	*/
	static Scene* SetActiveScene(int index);

	/**
	* Returns the size of the scenes vector
	*/
	static int GetScenesSize();

	/**
	* Returns the index of the scene where name matches, if scene cannot be found will return 0
	*/
	static int GetSceneIndex(std::string name);

	/**
	* Gets a pointer to a scene from the scenes vector at given index
	*/
	static Scene* GetScene(int index);

	/**
	* Get a pointer to the first scene from the scenes vector that matches name
	*/
	static Scene* GetScene(std::string name);

	/**
	* Add a scene to the scenes vector, then return scene ptr
	*/
	static Scene* AddScene(Scene* scene);

	/**
	* Removes the scene from the scenes vector
	*/
	static void RemoveScene(int index);

	/**
	* Destructor
	*/
	~SceneManager();
};

#endif // !SCENEMANAGER_H

