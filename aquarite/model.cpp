/**
*	Filename: model.cpp
*
*	Description: Source file for Model class.
*
*	Version: 15/1/2019
*
*	© 2018, Jens Heukers
*/
#include "model.h"

Model::Model() {
	this->name = "Unnamed Model";
	this->sphereRadius = 1.0f; // Default sphere radius
	this->ignoreFrustum = false; // we dont want to ignore frustum by default
	this->drawMode = DrawMode::Default; // set drawmode to default
}

Material* Model::GetMaterial(int index) {
	return this->materials[index];
}

void Model::AddMaterial(Material* material) {
	this->materials.push_back(material);
}

void Model::RemoveMaterial(int index) {
	this->materials.erase(this->materials.begin() + index);
}

int Model::GetMaterialCount() {
	return (int)this->materials.size();
}

Mesh* Model::GetMesh(int index) {
	return this->meshes[index];
}

void Model::AddMesh(Mesh* mesh) {
	this->meshes.push_back(mesh);
}

void Model::RemoveMesh(int index) {
	this->meshes.erase(this->meshes.begin() + index);
}

int Model::GetMeshesCount() {
	return (int)this->meshes.size();
}

void Model::SetName(std::string name) {
	this->name = name;
}

std::string Model::GetName() {
	return this->name;
}

float Model::GetSphereRadius() {
	return this->sphereRadius;
}

void Model::SetSphereRadius(float amount) {
	this->sphereRadius = amount;
}

bool Model::IgnoreFrustumState() {
	return this->ignoreFrustum;
}

void Model::IgnoreFrustum(bool state) {
	this->ignoreFrustum = state;
}

DrawMode Model::GetDrawMode() {
	return this->drawMode;
}

void Model::SetDrawMode(DrawMode mode) {
	this->drawMode = mode;
}