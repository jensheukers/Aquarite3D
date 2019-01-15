/**
*	Filename: entity.cpp
*
*	Description: Source file for Entity class
*
*	Version: 15/1/2019
*
*	© 2018, Jens Heukers
*/

#include "resourcemanager.h"
#include "entity.h"
#include "debug.h"

unsigned Entity::_currentId; // Declare static member

void Entity::UpdateChildren() {
	// Handle position/rotation/scale accoring to parent
	if (this->parent) { // If we have a parent
		// Set global position, rotation and scale
		this->globalPosition = Vec3(parent->globalPosition.x + position.x, parent->globalPosition.y + position.y, parent->globalPosition.z + position.z);
		this->globalRotation = Vec3(parent->globalRotation.x + localRotation.x, parent->globalRotation.y + localRotation.y, parent->globalRotation.z + localRotation.z);
		this->globalScale = Vec3(parent->globalScale.x + localScale.x, parent->globalScale.y + localScale.y, parent->globalScale.z + localScale.z);
	}
	else {
		// Set global position, rotation and scale
		this->globalPosition = position;
		this->globalRotation = localRotation;
		this->globalScale = localScale;
	}

	//Update children
	for (unsigned i = 0; i < children.size(); i++) {
		children[i]->UpdateChildren();
	}

	this->Update(); // Call local update function
}

void Entity::Render(Renderer* renderer, Camera* camera) {
	if (this->model != nullptr) {
		switch (this->renderMode) {
		case RenderMode::WorldSpace:
			renderer->RegisterEntity(this);
			break;
		case RenderMode::ScreenSpace:
			renderer->DrawSprite(model, this->globalPosition, this->globalRotation, localScale);
			break;
		}
	}

	for (unsigned i = 0; i < children.size(); i++) {
		children[i]->Render(renderer, camera);
	}
}

Entity::Entity(RenderMode renderMode, Shader* shader, Material* material) {
	if (!_currentId) { // If _currentId is not yet initialized
		_currentId = 0; // Set _currentId to 0;
	}

	this->parent = nullptr; // Set parent to nullptr
	this->model = nullptr; // Set model to nullptr
	this->id = _currentId; // Set this id to the _currentId
	this->renderMode = renderMode; // Set the render type

	//Set scale
	this->localScale = Vec3(1.0f,1.0f,1.0f);

	this->name = "Entity";
	_currentId++; // Increment global variable _currentId by 1
}

unsigned Entity::GetId() {
	return this->id; // Return the entity id
}

void Entity::SetName(std::string name) {
	this->name = name;
}

std::string Entity::GetName() {
	return this->name;
}

Entity* Entity::GetChild(int index) {
	return this->children[index]; // Return child
}

Entity* Entity::AddChild(Entity* child) {
	child->parent = this; // Set parent to this object
	children.push_back(child); // Push back child
	return child; //  Return child
}

void Entity::RemoveChild(Entity* entity) {
	int index = -1;
	for (size_t i = 0; i < this->children.size(); i++) {
		if (this->children[i] == entity) {
			index = i;
		}
	}

	if (index == -1) return;

	//Remove all children from child
	for (size_t i = 0; i < this->children[index]->children.size(); i++) {
		this->children[index]->children.erase(this->children[index]->children.begin() + i);
	}

	this->children.erase(this->children.begin() + index); // Erase
}

std::vector<Entity*> Entity::GetChildren() {
	return this->children; // Return children Vector Array
}

void Entity::Translate(Vec3 position) {
	this->position = Vec3(this->position.x + position.x, this->position.y + position.y, this->position.z + position.z);
}

Vec3 Entity::GetPositionGlobal() {
	return this->globalPosition; // Return global position
}

void Entity::SetRotation(Vec3 rotation) {
	//Regular checks
	if (rotation.x > 360) {
		rotation.x = 0;
	}

	if (rotation.y > 360) {
		rotation.y = 0;
	}

	if (rotation.z > 360) {
		rotation.z = 0;
	}

	if (rotation.x < 0) {
		rotation.x = 360;
	}

	if (rotation.y < 0) {
		rotation.y = 360;
	}

	if (rotation.z < 0) {
		rotation.z = 360;
	}

	// Set local rotation
	this->localRotation = rotation;
}

void Entity::Rotate(Vec3 rotation) {
	//Regular checks
	if (rotation.x > 360) {
		rotation.x = 0;
	}

	if (rotation.y > 360) {
		rotation.y = 0;
	}

	if (rotation.z > 360) {
		rotation.z = 0;
	}

	if (rotation.x < 0) {
		rotation.x = 360;
	}

	if (rotation.y < 0) {
		rotation.y = 360;
	}

	if (rotation.z < 0) {
		rotation.z = 360;
	}

	this->localRotation = Vec3(this->localRotation.x + rotation.x, this->localRotation.y + rotation.y, this->localRotation.z + rotation.z);
}

Vec3 Entity::GetRotation() {
	return this->localRotation; // Return local rotation
}

Vec3 Entity::GetRotationGlobal() {
	return this->globalRotation; // Return global rotation
}

void Entity::SetScale(Vec3 scale) {
	//Regular checks
	if (scale.x < 0) {
		scale.x = 0;
	}

	if (scale.y < 0) {
		scale.y = 0;
	}

	if (scale.z < 0) {
		scale.z = 0;
	}

	this->localScale = scale; // Set local scale
}

Vec3 Entity::GetScale() {
	return this->localScale; // Return local scale
}

Vec3 Entity::GetScaleGlobal() {
	return this->globalScale; //  Return global scale
}

void Entity::SetModel(Model* model) {
	this->model = model;
}

Model* Entity::GetModel() {
	return this->model;
}

void Entity::SetRenderMode(RenderMode renderMode) {
	this->renderMode = renderMode;
}

RenderMode Entity::GetRenderMode() {
	return this->renderMode;
}

void Entity::SetParent(Entity* parent) {
	if (parent != nullptr) {
		this->parent = parent;
	}
}

Entity* Entity::GetParent() {
	return this->parent;
}

Entity::~Entity() {
	for (size_t i = 0; i < children.size(); i++) {
		delete children[i];
	}

	Debug::Log("Deleted Entity:", typeid(*this).name());
	Debug::Log(std::to_string(this->GetId()), typeid(*this).name());
}


// Sprite Definitions

float Sprite::_defaultUvCoords[8] = { 1.0f, 0.0f,
									  0.0f, 0.0f, 
									  1.0f, 1.0f, 
									  0.0f, 1.0f }; // Declare static member

void Sprite::Create() {
	std::vector<float> _vertices = {
		// first triangle
		1.0f,  1.0f, 0.0f, _uvData._rightUp[0], _uvData._rightUp[1], // top right
		1.0f, -1.0f, 0.0f, _uvData._rightDown[0], _uvData._rightDown[1],  // bottom right
		-1.0f,  1.0f, 0.0f, _uvData._leftUp[0], _uvData._leftUp[1], // top left 
		// second triangle
		1.0f, -1.0f, 0.0f, _uvData._rightDown[0], _uvData._rightDown[1], // bottom right
		-1.0f, -1.0f, 0.0f, _uvData._leftDown[0], _uvData._leftDown[1], // bottom left
		-1.0f,  1.0f, 0.0f, _uvData._leftUp[0], _uvData._leftUp[1]   // top left
	};

	Model* model = new Model();
	Material* mat = new Material();
	mat->SetShader(ResourceManager::GetShader("_aquariteDefaultSpriteShader"));
	model->AddMaterial(mat);

	Mesh* mesh = new Mesh();
	mesh->GenerateBuffers(_vertices); // Generate buffers
	model->AddMesh(mesh); // Set the mesh active
}

// Default parameter is taken in if no parameter is passed
Sprite::Sprite(float _uvCoords[8], RenderMode renderMode) {
	//Set render mode
	this->SetRenderMode(renderMode);

	//Handle Uv Data
	UVData4p _uvData;
	_uvData._rightDown = glm::vec2(_defaultUvCoords[0],_defaultUvCoords[1]);
	_uvData._leftDown = glm::vec2(_defaultUvCoords[2], _defaultUvCoords[3]);
	_uvData._rightUp = glm::vec2(_defaultUvCoords[4], _defaultUvCoords[5]);
	_uvData._leftUp = glm::vec2(_defaultUvCoords[6], _defaultUvCoords[7]);
	this->_uvData = _uvData; // Set Uv Data
	this->Create();
}

Sprite::Sprite(UVData4p _uvData) {
	//Set UV data
	this->_uvData._leftUp = _uvData._leftUp;
	this->_uvData._rightUp = _uvData._rightUp;
	this->_uvData._leftDown = _uvData._leftDown;
	this->_uvData._rightDown = _uvData._rightDown;
	this->Create();
}