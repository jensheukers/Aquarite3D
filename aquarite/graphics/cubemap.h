/**
*	Filename: cubemap.h
*
*	Description: Header file for CubeMap class.
*
*	Version: 16/1/2019
*
*	© 2018, Jens Heukers
*/
#ifndef CUBEMAP_H
#define CUBEMAP_H
#include <GL/GL.h>
#include <vector>
#include "shader.h"
#include "../texture.h"

class CubeMap {
private:
	unsigned int cubeMapTexture; /// @brief The cube map texture object
	std::vector<Texture*> textures; /// @brief List of pointers to textures used
public:
	/**
	* Takes in a array of 6 textures to construct a cubemap,
	* Textures should be specified in following order:
	* FRONT, BACK, DOWN, UP, RIGHT, LEFT
	*/
	void ConstructCubeMapTexture(std::vector<Texture*> textureFaces);

	/**
	* Returns the Cube Map Texture
	*/
	unsigned int GetCubeMapTexture();

	/**
	* Returns a texture on specific index
	*/
	Texture* GetTexture(int index);
};

//Skybox class inherits from CubeMap, it is not neccesary to include
//a skybox class but for the ease of it we will
class SkyBox : public CubeMap {
private:
	unsigned int vao; /// @brief The vertex array object of the skybox
	unsigned int vbo; /// @brief The vertex buffer object of the skybox
	Shader* shader; /// @brief The shader used by the skybox, will be set to default in constructor

	/**
	* Generate the buffers for the skybox
	*/
	void GenerateBuffers();
public:
	/**
	* Constructor
	*/
	SkyBox();

	/**
	* Returns the vertex array object
	*/
	unsigned int GetVAO();

	/**
	* Returns the vertex buffer object
	*/
	unsigned int GetVBO();

	/**
	* Returns the shader object
	*/
	Shader* GetShader();

	/**
	* Sets the shader object
	*/
	void SetShader(Shader* shader);
};

#endif // !CUBEMAP_H