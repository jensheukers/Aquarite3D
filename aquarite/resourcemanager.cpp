/**
*	Filename: resourcemanager.cpp
*
*	Description: Source file for ResourceManager singleton class.
*
*	Version: 15/1/2019
*
*	© 2018, Jens Heukers
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include "core.h"
#include "model.h"
#include "resourcemanager.h"
#include "debug.h"

ResourceManager* ResourceManager::_instance; // declare instance

ResourceManager* ResourceManager::GetInstance() {
	if (_instance) {
		return _instance; // return instance
	} 
	_instance = new ResourceManager();

	// Add default shaders to shader list
	_instance->AddShader("_aquariteDefaultShader", new Shader("shaders/default.vs", "shaders/default.fs"));
	_instance->AddShader("_aquariteDefaultSpriteShader", new Shader("shaders/sprite_default.vs", "shaders/sprite_default.fs"));

	// Add default material to material list
	_instance->AddMaterial("_aquariteDefaultMaterial", new Material());

	Debug::Log("Instanciated", typeid(*ResourceManager::GetInstance()).name());
	return _instance;
}

void ResourceManager::AddTexture(std::string key, Texture* texture) {
	ResourceManager::GetInstance()->_textures[key] = texture;

	std::string _formattedMsg = "Added Texture Resource: ";
	_formattedMsg.append(key);
	Debug::Log(_formattedMsg, typeid(*ResourceManager::GetInstance()).name());
}

Texture* ResourceManager::GetTexture(std::string key) {
	return ResourceManager::GetInstance()->_textures[key];
}

void ResourceManager::RemoveTexture(std::string key) {
	if (ResourceManager::GetInstance()->_textures[key] == nullptr) return;
	delete ResourceManager::GetTexture(key);
	
	ResourceManager::GetInstance()->_textures[key] = nullptr;

	std::string _convertedString = "Removed Texture resource: ";
	_convertedString.append(key);
	Debug::Log(_convertedString, typeid(*ResourceManager::GetInstance()).name());
}

void ResourceManager::AddMesh(std::string key, Mesh* mesh) {
	ResourceManager::GetInstance()->_meshes[key] = mesh;

	std::string _formattedMsg = "Added Mesh Resource: ";
	_formattedMsg.append(key);
	Debug::Log(_formattedMsg, typeid(*ResourceManager::GetInstance()).name());
}

Mesh* ResourceManager::GetMesh(std::string key) {
	return ResourceManager::GetInstance()->_meshes[key];
}

void ResourceManager::RemoveMesh(std::string key) {
	if (ResourceManager::GetInstance()->_meshes[key] == nullptr) return;
	delete ResourceManager::GetMesh(key);

	ResourceManager::GetInstance()->_meshes[key] = nullptr;

	std::string _convertedString = "Removed Mesh resource: ";
	_convertedString.append(key);
	Debug::Log(_convertedString, typeid(*ResourceManager::GetInstance()).name());
}

void ResourceManager::AddShader(std::string key, Shader* shader) {
	ResourceManager::GetInstance()->_shaders[key] = shader;

	std::string _formattedMsg = "Added Shader Resource: ";
	_formattedMsg.append(key);
	Debug::Log(_formattedMsg, typeid(*ResourceManager::GetInstance()).name());
}

Shader* ResourceManager::GetShader(std::string key) {
	return ResourceManager::GetInstance()->_shaders[key];
}

void ResourceManager::RemoveShader(std::string key) {
	if (ResourceManager::GetInstance()->_shaders[key] == nullptr) return;
	delete ResourceManager::GetShader(key);

	ResourceManager::GetInstance()->_shaders[key] = nullptr;

	std::string _convertedString = "Removed Shader resource: ";
	_convertedString.append(key);
	Debug::Log(_convertedString, typeid(*ResourceManager::GetInstance()).name());
}

void ResourceManager::AddMaterial(std::string key, Material* material) {
	ResourceManager::GetInstance()->_materials[key] = material;

	std::string _formattedMsg = "Added Material Resource: ";
	_formattedMsg.append(key);
	Debug::Log(_formattedMsg, typeid(*ResourceManager::GetInstance()).name());
}

Material* ResourceManager::GetMaterial(std::string key) {
	return ResourceManager::GetInstance()->_materials[key];
}

void ResourceManager::RemoveMaterial(std::string key) {
	if (ResourceManager::GetInstance()->_materials[key] == nullptr) return;
	delete ResourceManager::GetMaterial(key);

	ResourceManager::GetInstance()->_materials[key] = nullptr;

	std::string _convertedString = "Removed Material resource: ";
	_convertedString.append(key);
	Debug::Log(_convertedString, typeid(*ResourceManager::GetInstance()).name());
}

void ResourceManager::AddModel(std::string key, Model* material) {
	ResourceManager::GetInstance()->_models[key] = material;

	std::string _formattedMsg = "Added Model Resource: ";
	_formattedMsg.append(key);
	Debug::Log(_formattedMsg, typeid(*ResourceManager::GetInstance()).name());
}

Model* ResourceManager::GetModel(std::string key) {
	return ResourceManager::GetInstance()->_models[key];
}

void ResourceManager::RemoveModel(std::string key) {
	if (ResourceManager::GetInstance()->_models[key] == nullptr) return;
	delete ResourceManager::GetModel(key);

	ResourceManager::GetInstance()->_models[key] = nullptr;

	std::string _convertedString = "Removed Model resource: ";
	_convertedString.append(key);
	Debug::Log(_convertedString, typeid(*ResourceManager::GetInstance()).name());
}

void ResourceManager::LoadMeta(std::string offset) {
	if (offset == "") return; // Return if size is less then 1
	
	//Read meta
	std::string metaPath = Core::GetBuildDirectory();
	metaPath.append(offset);

	std::string metaLine;
	std::ifstream metaFile = std::ifstream(metaPath);

	if (metaFile.is_open()) {
		Meta* currentMeta = nullptr;
		std::string curType = "";

		while (std::getline(metaFile, metaLine)) {
			if (metaLine == "") continue;
			if (metaLine.at(0) == '/' && metaLine.at(1) == '/') continue; //Comment

			if (metaLine == "#META") {
				currentMeta = new Meta();
				curType = metaLine;
				continue;
			}

			if (metaLine == "#TEXTURES" || metaLine == "#MESHES" || metaLine == "#MATERIALS" || metaLine == "#MODELS") {
				curType = metaLine;
				continue;
			}

			if (currentMeta == nullptr) continue;

			//We know we can split here at character =

			std::stringstream metaSs(metaLine);
			std::string metaSegment;
			std::vector<std::string> metaSegments;
			while (std::getline(metaSs, metaSegment, '=')) {
				metaSegments.push_back(metaSegment);
			}

			if (curType == "#META") {
				if (metaSegments[0] == "resourceName") {
					currentMeta->resourceName = metaSegments[1];
					continue;
				}

				if (metaSegments[0] == "offset") {
					currentMeta->offset = metaSegments[1];
					continue;
				}

				if (metaSegments[0] == "author") {
					currentMeta->author = metaSegments[1];
					continue;
				}

				if (metaSegments[0] == "version") {
					currentMeta->version = std::stof(metaSegments[1]);
					continue;
				}
				continue;
			}

			if (curType == "#TEXTURES") {
				std::string _path = Core::GetBuildDirectory();
				_path.append(currentMeta->offset);
				_path.append(metaSegments[1]);

				Texture* texture = new Texture();
				texture->LoadTGA((char*)_path.c_str());

				ResourceManager::AddTexture(metaSegments[0], texture);
				continue;
			}

			if(curType == "#MESHES") {
				std::string _path = Core::GetBuildDirectory();
				_path.append(currentMeta->offset);
				_path.append(metaSegments[1]);

				Mesh* mesh = new Mesh();
				mesh->LoadObj(_path);

				ResourceManager::AddMesh(metaSegments[0], mesh);
				continue;
			}

			if (curType == "#MATERIALS") {
				std::string _path = Core::GetBuildDirectory();
				_path.append(currentMeta->offset);
				_path.append(metaSegments[1]);

				std::string line;
				std::ifstream file = std::ifstream(_path);

				if (file.is_open()) {
					Material* currentMaterial;
					while (std::getline(file, line))
					{
						if (line == "") continue;

						if (line == "#MAT") {
							currentMaterial = new Material();
							continue;
						}

						if (!currentMaterial) continue;

						std::stringstream ss(line);
						std::string segment;
						std::vector<std::string> segments;
						while (std::getline(ss, segment, '=')) {
							segments.push_back(segment);
						}

						if (segments[0] == "name") {
							ResourceManager::AddMaterial(segments[1], currentMaterial);
							continue;
						}

						if (segments[0] == "diffuseMap") {
							currentMaterial->SetDiffuse(ResourceManager::GetTexture(segments[1]));
							continue;
						}

						if (segments[0] == "shader") {
							currentMaterial->SetShader(ResourceManager::GetShader(segments[1]));
							continue;
						}

						if (segments[0] == "shininess") {
							currentMaterial->SetShine(std::stof(segments[1]));
							continue;
						}

						//If reached this point we can assume that comma's are existant in values so we can split

						std::stringstream ss2(segments[1]);
						std::string segment2;
						std::vector<std::string> commas;
						while (std::getline(ss2, segment2, ',')) {
							commas.push_back(segment2);
						}


						if (segments[0] == "color") {
							currentMaterial->SetColor(glm::vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
							continue;
						}

						if (segments[0] == "diffuseColor") {
							currentMaterial->SetDiffuseColor(glm::vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
							continue;
						}

						if (segments[0] == "ambientColor") {
							currentMaterial->SetAmbientColor(glm::vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
							continue;
						}

						if (segments[0] == "specular") {
							currentMaterial->SetSpecular(glm::vec3(std::stof(commas[0]), std::stof(commas[1]), std::stof(commas[2])));
							continue;
						}
					}
					file.close();
				}
			}

			if (curType == "#MODELS") {
				std::string _path = Core::GetBuildDirectory();
				_path.append(currentMeta->offset);
				_path.append(metaSegments[1]);

				std::string line;
				std::ifstream file;
				file = std::ifstream(_path);

				if (file.is_open()) {
					Model* currentModel;
					while (std::getline(file, line))
					{
						if (line == "") continue;

						if (line == "#MODEL") {
							currentModel = new Model();
							continue;
						}

						std::stringstream ss(line);
						std::string segment;
						std::vector<std::string> segments;
						while (std::getline(ss, segment, '=')) {
							segments.push_back(segment);
						}

						if (segments[0] == "name") {
							ResourceManager::AddModel(segments[1], currentModel);
							currentModel->SetName(segments[1]);
						}

						if (segments[0] == "mesh") {
							currentModel->AddMesh(ResourceManager::GetMesh(segments[1]));
						}

						if (segments[0] == "material") {
							currentModel->AddMaterial(ResourceManager::GetMaterial(segments[1]));
						}

						if (segments[0] == "radius") {
							currentModel->SetSphereRadius(std::stof(segments[1]));
						}

						if (segments[0] == "ignoreFrustum") {
							currentModel->IgnoreFrustum(std::stoi(segments[1]));
						}
					}
					file.close();
				}
			}
		}
		metaFile.close();
	}
}

void ResourceManager::UnLoad() {
	//Safely delete all resources
	//Textures
	std::map<std::string, Texture*>::iterator tex_it;
	for (tex_it = ResourceManager::GetInstance()->_textures.begin(); tex_it != ResourceManager::GetInstance()->_textures.end(); ++tex_it) {
		if (tex_it->second != nullptr) RemoveTexture(tex_it->first);
	}

	//Meshes
	std::map<std::string, Mesh*>::iterator mesh_it;
	for (mesh_it = ResourceManager::GetInstance()->_meshes.begin(); mesh_it != ResourceManager::GetInstance()->_meshes.end(); ++mesh_it) {
		if (mesh_it->second != nullptr) RemoveMesh(mesh_it->first);
	}

	//Shaders
	std::map<std::string, Shader*>::iterator shad_it;
	for (shad_it = ResourceManager::GetInstance()->_shaders.begin(); shad_it != ResourceManager::GetInstance()->_shaders.end(); ++shad_it) {
		if (shad_it->second != nullptr) RemoveShader(shad_it->first);
	}

	//Materials
	std::map<std::string, Material*>::iterator mat_it;
	for (mat_it = ResourceManager::GetInstance()->_materials.begin(); mat_it != ResourceManager::GetInstance()->_materials.end(); ++mat_it) {
		if (mat_it->second != nullptr) RemoveMaterial(mat_it->first);
	}

	//Models
	std::map<std::string, Model*>::iterator model_it;
	for (model_it = ResourceManager::GetInstance()->_models.begin(); model_it != ResourceManager::GetInstance()->_models.end(); ++model_it) {
		if (model_it->second != nullptr) RemoveModel(model_it->first);
	}
}

ResourceManager::~ResourceManager() {
	ResourceManager::UnLoad(); // Unload resources
}