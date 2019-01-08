/**
*	Filename: material.h
*
*	Description: Header file for Material class, Lighting is based among the Phong model
*
*	Version: 21/12/2018
*
*	© 2018, Jens Heukers
*/
#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/glm.hpp>
#include "../resourcemanager.h"
#include "../texture.h"
#include "shader.h"

class Material {
private:
	Texture* diffuseMap;
	Shader* shader;
	glm::vec3 color; /// @brief Vec3 containing color x,y,z.
	glm::vec3 diffuseColor; /// @brief Vec3 containing diffuse x,y,z.
	glm::vec3 ambientColor; /// @brief Vec3 containing ambient x,y,z.
	glm::vec3 specular; /// @brief Vec3 containing specular x,y,z.
	float shininess; /// @brief Float containing shine value.
public:
	/**
	* Constructor, takes as default parameters (glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.5f), 32f).
	*/
	Material(Shader* shader = ResourceManager::GetShader("_aquariteDefaultShader"),glm::vec3 color = glm::vec3(1.0f), glm::vec3 diffuseColor = glm::vec3(1.0f), glm::vec3 ambientColor = glm::vec3(1.0f), glm::vec3 specular = glm::vec3(0.5f), float shininess = 32.0f);

	/**
	* Set the diffuse map of the material
	*/
	void SetDiffuse(Texture* texture);

	/**
	* Get the diffuse map of the material
	*/
	Texture* GetDiffuse();

	/**
	* Set the shader of the material
	*/
	void SetShader(Shader* shader);

	/**
	* Get the shader of the material
	*/
	Shader* GetShader();

	/**
	* Set the color of the material.
	*/
	void SetColor(glm::vec3 color);

	/**
	* Get the color of the material
	*/
	glm::vec3 GetColor();

	/**
	* Set the diffuse of the material.
	*/
	void SetDiffuseColor(glm::vec3 diffuse);

	/**
	* Get the color of the material
	*/
	glm::vec3 GetDiffuseColor();

	/**
	* Set the Ambient Color of the material.
	*/
	void SetAmbientColor(glm::vec3 ambient);

	/**
	* Get the Ambient Color of the material
	*/
	glm::vec3 GetAmbientColor();

	/**
	* Set the specular of the material.
	*/
	void SetSpecular(glm::vec3 specular);

	/**
	* Get the color of the material
	*/
	glm::vec3 GetSpecular();

	/**
	* Set the shininess of the material.
	*/
	void SetShine(float specular);

	/**
	* Get the shininess of the material
	*/
	float GetShine();
};

#endif // !MATERIAL_H