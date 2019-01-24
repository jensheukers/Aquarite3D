/**
*	Filename: scene.cpp
*
*	Description: Source file for Scene class
*
*	Version: 15/1/2019
*
*	© 2018, Jens Heukers
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include "scene.h"
#include "core.h"
#include "graphics/light.h"

Scene::~Scene() {
	for (size_t i = 0; i < GetChildren().size(); i++) {
		if (static_cast<Light*>(GetChild(i))) {
			RemoveLight(static_cast<Light*>(GetChild(i)));
		}
	}
}

Scene::Scene() {
	this->SetName("Unnamed_Scene"); // Give scene a little name
	this->activeCamera = nullptr; // set active camera to nullptr
}

void Scene::UpdateSceneChildren() {
	this->UpdateChildren(); // Update children
	this->Update(); // call Update function
}

void Scene::RenderSceneChildren(Renderer* renderer, Camera* camera) {
	this->Render(renderer, camera); // Render all the scene Children
}

Camera* Scene::GetActiveCamera() {
	return this->activeCamera;
}

void Scene::SetActiveCamera(Camera* camera) {
	this->activeCamera = camera;
}

void Scene::AddLight(Light* light) {
	Core::GetInstance()->HandleLightRegister(light, 0);
	this->AddChild(light);
}

void Scene::RemoveLight(Light* light) {
	Core::GetInstance()->HandleLightRegister(light, 1);
	this->RemoveChild(light);
}

void Scene::LoadSceneData(std::string offset) {

	std::string path = Core::GetBuildDirectory();
	path.append(offset);

	//Temp variables
	std::string type = "";

	Entity* entityptr = nullptr; // Entity pointer
	Light* lightptr = nullptr; // Light pointer
	DirectionalLight* dirLightptr = nullptr; // Dir light pointer

	std::string line;
	std::ifstream file = std::ifstream(path);

	if (file.is_open()) {
		while (std::getline(file, line))
		{
			if (line == "") continue;

			if (line == "#HEADER") {
				type = line;
			}

			//Entity
			if (line == "#ENTITY") {
				entityptr = new Entity();
				this->AddChild(entityptr);
				type = line;
				continue;
			}

			//Light
			if (line == "#LIGHT") {
				lightptr = new Light();
				lightptr->SetLightType(LightType::PointLight);
				this->AddLight(lightptr);
				type = line;
				continue;
			}

			//Directional
			if (line == "#LIGHT_DIRECTIONAL") {
				dirLightptr = new DirectionalLight();
				this->AddLight(dirLightptr);
				type = line;
				continue;
			}

			//We know that we can split at '='
			std::stringstream ss(line);
			std::string segment;
			std::vector<std::string> segments;
			while (std::getline(ss, segment, '=')) {
				segments.push_back(segment);
			}

			//HEADER
			if (type == "#HEADER") {
				if (segments[0] == "name") {
					this->SetName(segments[1]);
				}
			}

			//Entity
			if (type == "#ENTITY") {
				if (segments[0] == "model") { // Set model
					entityptr->SetModel(ResourceManager::GetModel(segments[1]));
				}

				//NOTE: Parent should be defined and added to scene before setting
				if (segments[0] == "parent") {
					if (std::stoi(segments[1]) != -1) {
						entityptr->SetParent(this->GetChild(std::stoi(segments[1])));
					}
				}

				// we know we can split at comma
				std::stringstream ss2(segments[1]);
				std::string comma;
				std::vector<std::string> commas;
				while (std::getline(ss2, comma, ',')) {
					commas.push_back(comma);
				}

				if (segments[0] == "position") {
					entityptr->position = Vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2]));
				}

				if (segments[0] == "rotation") {
					entityptr->SetRotation(Vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
				}

				if (segments[0] == "scale") {
					entityptr->SetScale(Vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
				}
			}

			//Light
			if (type == "#LIGHT") {
				// we know we can split at comma
				std::stringstream ss2(segments[1]);
				std::string comma;
				std::vector<std::string> commas;
				while (std::getline(ss2, comma, ',')) {
					commas.push_back(comma);
				}

				if (segments[0] == "position") {
					lightptr->position = Vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2]));
				}

				if (segments[0] == "ambient") {
					lightptr->SetAmbient(glm::vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
				}

				if (segments[0] == "diffuse") {
					lightptr->SetDiffuse(glm::vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
				}

				if (segments[0] == "specular") {
					lightptr->SetSpecular(glm::vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
				}
			}

			//Direcitonal
			if (type == "#LIGHT_DIRECTIONAL") {
				// we know we can split at comma
				std::stringstream ss2(segments[1]);
				std::string comma;
				std::vector<std::string> commas;
				while (std::getline(ss2, comma, ',')) {
					commas.push_back(comma);
				}

				if (segments[0] == "direction") {
					dirLightptr->SetDirection(glm::vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
				}

				if (segments[0] == "ambient") {
					dirLightptr->SetAmbient(glm::vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
				}

				if (segments[0] == "diffuse") {
					dirLightptr->SetDiffuse(glm::vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
				}

				if (segments[0] == "specular") {
					dirLightptr->SetSpecular(glm::vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
				}
			}
		}
		file.close();
	}
}