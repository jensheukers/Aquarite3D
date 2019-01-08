/**
*	Filename: light.h
*
*	Description: Header file for Light class.
*
*	Version: 24/12/2018
*
*	© 2018, Jens Heukers
*/
#ifndef LIGHT_H
#define LIGHT_H
#include "../entity.h"
#include <glm/glm.hpp>

/**
* Enum containing available Light Types
*/
enum LightType {
	NONE,
	Directional,
	PointLight
};

class Light : public Entity {
private:
	LightType type; /// @brief The type of the light

	glm::vec3 ambient; /// @brief The Ambient Component of the light
	glm::vec3 diffuse; /// @brief The Diffuse Component of the light
	glm::vec3 specular; /// @brief The Specular Component of the light
public:
	/**
	* Constructor, Type is default set to None, so light type has to be set manually
	*/
	Light(LightType type = LightType::NONE);

	//Getters/setters

	/**
	* Set the Light Type
	*/
	void SetLightType(LightType type);

	/**
	* Get the Light Type
	*/
	LightType GetLightType();

	/**
	* Set Ambient
	*/
	void SetAmbient(glm::vec3 ambient);

	/**
	* Get Ambient
	*/
	glm::vec3 GetAmbient();

	/**
	* Set Diffuse
	*/
	void SetDiffuse(glm::vec3 ambient);

	/**
	* Get Ambient
	*/
	glm::vec3 GetDiffuse();

	/**
	* Set Specular
	*/
	void SetSpecular(glm::vec3 ambient);

	/**
	* Get Specular
	*/
	glm::vec3 GetSpecular();
};

//Light Types

class DirectionalLight : public Light {
private:
	glm::vec3 direction; /// @brief Direction Component
public:

	/**
	* Constructor sets type to DirectionalLight
	*/
	DirectionalLight();

	/**
	* Set Specular
	*/
	void SetDirection(glm::vec3 ambient);

	/**
	* Get Specular
	*/
	glm::vec3 GetDirection();
};

#endif // !LIGHT_H
