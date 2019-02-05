/**
*	Filename: cubemap.cpp
*
*	Description: Header file for CubeMap class.
*
*	Version: 16/1/2019
*
*	© 2018, Jens Heukers
*/
#include <GL/glew.h>
#include "cubemap.h"
#include "../debug.h"
#include "../resourcemanager.h"

void CubeMap::ConstructCubeMapTexture(std::vector<Texture*> textureFaces) {
	Debug::Log("Loading Cubemap Texture", typeid(*this).name());
	glGenTextures(1, &cubeMapTexture);
	glBindTexture(GL_TEXTURE_2D, cubeMapTexture);

	if (textureFaces.size() < 6 || textureFaces.size() > 6) {
		Debug::Log("Error: Texture faces vector does not have size of 6", typeid(*this).name());
		return;
	}

	//We can assume that all texture faces sizes are equal,
	// So we can take the data of texturefaces[0].
	int width = textureFaces[0]->textureData->width;
	int height = textureFaces[0]->textureData->height;
	for (size_t i = 0; i < textureFaces.size(); i++) {
		//If size does not match return
		if (textureFaces[i]->textureData->width != width || textureFaces[i]->textureData->height != height) {
			Debug::Log("Error: one or more texture faces do not equal the height or width of textureFaces[0]", typeid(*this).name());
			return;
		}

		//Get the data from the texture, and insert
		GLubyte* data = textureFaces[i]->textureData->imageData;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);

		if (this->textures.size() >= 6) {
			this->textures.erase(this->textures.begin(), this->textures.begin() + i);
		}

		this->textures.push_back(textureFaces[i]);
	}

	//Set texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	Debug::Log("Succesfully created Cubemap texture", typeid(*this).name());
}

unsigned int CubeMap::GetCubeMapTexture() {
	return this->cubeMapTexture;
}

Texture* CubeMap::GetTexture(int index) {
	if (this->textures.size() < (size_t)index) {
		return nullptr;
	}

	return this->textures[index];
}

//Skybox class

void SkyBox::GenerateBuffers() {
	float vertices[] = {         
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	//Generate vertex array object and vertex buffer object
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	//Bind
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Buffer data and set vertex attrib pointers
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

SkyBox::SkyBox() {
	this->shader = ResourceManager::GetShader("_aquariteDefaultSkyBoxShader");
	GenerateBuffers();
}

unsigned int SkyBox::GetVAO() {
	return this->vao;
}

unsigned int SkyBox::GetVBO() {
	return this->vbo;
}

Shader* SkyBox::GetShader() {
	return this->shader;
}

void SkyBox::SetShader(Shader* shader) {
	this->shader = shader;
}