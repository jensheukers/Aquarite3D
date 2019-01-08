/**
*	Filename: material.cpp
*
*	Description: Source file for Material class.
*
*	Version: 21/12/2018
*
*	© 2018, Jens Heukers
*/
#include "material.h"

Material::Material(Shader* shader,glm::vec3 color, glm::vec3 diffuseColor, glm::vec3 ambientColor, glm::vec3 specular, float shininess) {
	this->shader = shader;
	this->color = color;
	this->diffuseColor = diffuseColor;
	this->ambientColor = ambientColor;
	this->specular = specular;
	this->shininess = shininess;
	this->diffuseMap = nullptr;
}


void Material::SetDiffuse(Texture* texture) {
	this->diffuseMap = texture;
}

Texture* Material::GetDiffuse() {
	return this->diffuseMap;
}

void Material::SetShader(Shader* shader) {
	this->shader = shader;
}

Shader* Material::GetShader() {
	return this->shader;
}

void Material::SetColor(glm::vec3 color) {
	this->color = color;
}

glm::vec3 Material::GetColor() {
	return this->color;
}

void Material::SetDiffuseColor(glm::vec3 diffuse) {
	this->diffuseColor = diffuse;
}

glm::vec3 Material::GetDiffuseColor() {
	return this->diffuseColor;
}

void Material::SetAmbientColor(glm::vec3 ambient) {
	this->ambientColor = ambient;
}

glm::vec3 Material::GetAmbientColor() {
	return this->ambientColor;
}

void Material::SetSpecular(glm::vec3 specular) {
	this->specular = specular;
}

glm::vec3 Material::GetSpecular() {
	return this->specular;
}

void Material::SetShine(float shine) {
	this->shininess = shine;
}

float Material::GetShine() {
	return this->shininess;
}