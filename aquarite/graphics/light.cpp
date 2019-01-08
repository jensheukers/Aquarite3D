/**
*	Filename: light.cpp
*
*	Description: Source file for Light class.
*
*	Version: 23/12/2018
*
*	© 2018, Jens Heukers
*/
#include "light.h"

Light::Light(LightType type) {
	this->SetName("Light");
	this->SetLightType(type); // Set light type
	
	// Set default values
	this->SetAmbient(glm::vec3(0.2f));
	this->SetDiffuse(glm::vec3(0.5f));
	this->SetSpecular(glm::vec3(1.0f));
}

void Light::SetLightType(LightType type) {
	this->type = type;
}

LightType Light::GetLightType() {
	return this->type;
}

void Light::SetAmbient(glm::vec3 ambient) {
	this->ambient = ambient;
}

glm::vec3 Light::GetAmbient() {
	return this->ambient;
}

void Light::SetDiffuse(glm::vec3 diffuse) {
	this->diffuse = diffuse;
}

glm::vec3 Light::GetDiffuse() {
	return this->diffuse;
}

void Light::SetSpecular(glm::vec3 specular) {
	this->specular = specular;
}

glm::vec3 Light::GetSpecular() {
	return this->specular;
}

//Directional Light

DirectionalLight::DirectionalLight() {
	this->SetLightType(LightType::Directional);
}

void DirectionalLight::SetDirection(glm::vec3 direction) {
	this->direction = direction;
}

glm::vec3 DirectionalLight::GetDirection() {
	return this->direction;
}