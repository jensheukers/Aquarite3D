/**
*	Filename: scenemanager.cpp
*
*	Description: Source file for SceneManager class
*
*	Version: 4/12/2018
*
*	© 2018, Jens Heukers
*/
#include "scenemanager.h"
#include "debug.h"

SceneManager* SceneManager::_instance; // Declare instance

SceneManager::SceneManager() {
	this->activeScene = nullptr; // Set active scene to nullptr
	Debug::Log("Instanciated", typeid(*this).name());
}

SceneManager* SceneManager::GetInstance() {
	if (!_instance) { // If instance is nullptr
		_instance = new SceneManager(); // Create new instance
	}
	
	return _instance; // Else return the instance
}

Scene* SceneManager::GetActiveScene() {
	return SceneManager::GetInstance()->activeScene; // Return active scene
}

Scene* SceneManager::SetActiveScene(int index) {
	// Check if scene exists
	if (SceneManager::GetInstance()->scenes[index]) {
		SceneManager::GetInstance()->activeScene = SceneManager::GetInstance()->scenes[index];
		Debug::Log("Active scene has been set to:", typeid(*SceneManager::GetInstance()).name());
		Debug::Log(SceneManager::GetInstance()->activeScene->GetName().c_str(), typeid(*SceneManager::GetInstance()).name());
	}
	else {
		SceneManager::GetInstance()->activeScene = nullptr; // If not set to nullptr
		Debug::Log("Could not set scene, index not found. Active Scene is nullptr", typeid(*SceneManager::GetInstance()).name());
	}
	return SceneManager::GetInstance()->activeScene;
}

int SceneManager::GetScenesSize() {
	return SceneManager::GetInstance()->scenes.size(); // return size
}

int SceneManager::GetSceneIndex(std::string name) {
	for (size_t i = 0; i < SceneManager::GetInstance()->scenes.size(); i++) {
		if (SceneManager::GetInstance()->scenes[i]->GetName() == name) { // If found
			return i; // Return index
		}
	}
	Debug::Log("Could not get Scene index, returning index 0", typeid(*SceneManager::GetInstance()).name());
	return 0; // Return 0
}

Scene* SceneManager::GetScene(int index) {
	return SceneManager::GetInstance()->scenes[index];
}

Scene* SceneManager::GetScene(std::string name) {
	return SceneManager::GetInstance()->GetScene(SceneManager::GetInstance()->GetSceneIndex(name)); // Return scene
}

Scene* SceneManager::AddScene(Scene* scene) {
	SceneManager::GetInstance()->scenes.push_back(scene); // Add Scene
	return scene; // Return scene
}

void SceneManager::RemoveScene(int index) {
	SceneManager::GetInstance()->scenes.erase(SceneManager::GetInstance()->scenes.begin() + index); // Remove scene from vector
}

SceneManager::~SceneManager() {
	for (size_t i = 0; i < scenes.size(); i++) {
		delete scenes[i]; // Delete all scenes
	}
}