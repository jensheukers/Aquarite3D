/**
*	Filename: texture.cpp
*
*	Description: Source file for Texture class, ported from Project Dynamite
*
*	Version: 23/1/2019
*
*	© 2018, Jens Heukers
*/
#ifdef _CRT_SECURE_NO_WARNINGS
#undef _CRT_SECURE_NO_WARNINGS
#endif
#define _CRT_SECURE_NO_WARNINGS 1

#include <Windows.h>
#include <string>
#include "texture.h"
#include "debug.h"

void Texture::BGR2RGB() {
	int bufferSize = (this->textureData->width * this->textureData->height) * this->textureData->bytesPerPixel;

	for (int i = 0; i < bufferSize; i += this->textureData->bytesPerPixel) {
		int b = this->textureData->imageData[i];
		int g = this->textureData->imageData[i + 1];
		int r = this->textureData->imageData[i + 2];

		this->textureData->imageData[i] = r;
		this->textureData->imageData[i + 1] = g;
		this->textureData->imageData[i + 2] = b;
	}
}

void Texture::UploadToGPU() {
	if (_glTexture) {
		glDeleteTextures(1, &this->_glTexture); // Delete the texture if already uploaded before
	}

	glGenTextures(1, &this->_glTexture); // Generate OpenGL Ready Textures

										 // Map the surface to the texture in video memory
	glBindTexture(GL_TEXTURE_2D, this->_glTexture);

	if (textureData->type == GL_RGB) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->width, textureData->height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData->imageData);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureData->width, textureData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData->imageData);
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

bool Texture::LoadTGA(char* filepath) {
	FILE* fTGA; //Declare file pointer
	fTGA = fopen(filepath, "rb"); //Open file for reading

	if (fTGA == NULL) { //If error was found..
		Debug::Log("Could not open: ", typeid(*this).name());
		Debug::Log(filepath, typeid(*this).name()); //Print error
		return false; //Return false
	}

	if (fread(&header, sizeof(header), 1, fTGA) == 0) { //If failure to read file header
		Debug::Log("Error reading file header: ", typeid(*this).name());
		Debug::Log(filepath, typeid(*this).name()); //Print error
		return false; //Return false
	}

	textureData = new TextureData();
	if (fread(targa.header, sizeof(targa.header), 1, fTGA) == 0) { //Attempt to read next 6 bytes
		Debug::Log("Error reading TGA: ", typeid(*this).name());
		Debug::Log(filepath, typeid(*this).name()); //Print error
		return false; // Return false
	}

	textureData->width = targa.header[1] * 256 + targa.header[0]; //Calculate height
	textureData->height = targa.header[3] * 256 + targa.header[2]; //Calculate width
	textureData->bpp = targa.header[4];
	targa.width = textureData->width;
	targa.height = textureData->height;
	targa.bpp = textureData->bpp;

	if ((textureData->width <= 0) || (textureData->height <= 0) || ((textureData->bpp != 24) && (textureData->bpp != 32))) { 	// Make Sure All Information Is Valid
		Debug::Log("Targa data is invalid! : ", typeid(*this).name());
		Debug::Log(filepath, typeid(*this).name()); //Print error
		return false;               // Return False
	}

	if (textureData->bpp == 32) { // if its a 32bpp image
		textureData->type = GL_RGBA; //Set type to RGBA
	}
	else { // else it must be 24bpp Image
		textureData->type = GL_RGB; //Set type to RGB
	}

	targa.bytesPerPixel = (targa.bpp / 8); // Calculate the BYTES per pixel
	targa.imageSize = (targa.bytesPerPixel * targa.width * targa.height); //Calculate the memory needed to store the image

	textureData->bytesPerPixel = targa.bytesPerPixel;

	textureData->imageData = (GLubyte*)malloc(targa.imageSize); //Allocate memory

	if (textureData->imageData == NULL) { // Check if imageData was allocated correctly
		Debug::Log("textureData was not allocated correctly : ", typeid(*this).name());
		Debug::Log(filepath, typeid(*this).name()); //Print error
		return false; // If Not, Return False
	}

	if (fread(textureData->imageData, 1, targa.imageSize, fTGA) != targa.imageSize) { 	// Attempt To Read All The Image Data
		Debug::Log("cant read image data : ", typeid(*this).name());
		Debug::Log(filepath, typeid(*this).name()); //Print error
		return false; //If we cant read the data return false
	}

	this->BGR2RGB(); //Convert from BGR to RGB
	this->UploadToGPU();

	Debug::Log("Texture created succesfully! Texture bits per pixel = ", typeid(*this).name());
	Debug::Log(std::to_string(textureData->bpp), typeid(*this).name()); //Print error

	fclose(fTGA);                   // Close The File
	return true;                    // Return Success
}

GLuint Texture::GetGLTexture() {
	return this->_glTexture;
}

int Texture::GetPixelData(int x, int y, int offset) {
	if (offset > ((int)this->textureData->bytesPerPixel - 1)) {
		offset = (this->textureData->bytesPerPixel - 1);
	}

	if (offset < 0) {
		offset = 0;
	}

	int height = (y * this->textureData->width) * this->textureData->bytesPerPixel;
	int width = x * this->textureData->bytesPerPixel;

	return this->textureData->imageData[(width + height) + offset];
}

void Texture::SetColor(Point4f color) {
	int bufferSize = (this->textureData->height * this->textureData->width) * this->textureData->bytesPerPixel; // Get the buffer size
	for (int i = 0; i < bufferSize; i += this->textureData->bytesPerPixel) { // For every pixel
		if (this->textureData->imageData[i] == 0 && this->textureData->imageData[i + 1] == 0 && this->textureData->imageData[i + 2] == 0) { // If a pixel has no color
			continue; //Continue
		}

		this->textureData->imageData[i] = (GLubyte)color.x; // Set Red
		this->textureData->imageData[i + 1] = (GLubyte)color.y; // Set Green
		this->textureData->imageData[i + 2] = (GLubyte)color.z; // Set Blue
		
		if (this->textureData->type == GL_RGBA) {
			this->textureData->imageData[i + 3] = (GLubyte)color.w; // Set alpha
		}
	}

	this->UploadToGPU(); //Re-Upload the texture
}

void Texture::GenerateTexture(int width, int height, GLuint type) {
	this->textureData = new TextureData();
	
	//Set some properties
	if (type == GL_RGB)
		this->textureData->bpp = 24; // 3 channels
	else
		this->textureData->bpp = 32; // 4 channels
	this->textureData->width = width;
	this->textureData->height = height;
	this->textureData->bytesPerPixel = (this->textureData->bpp / 8);
	this->textureData->type = type;

	//Allocate
	float imageSize = (float)this->textureData->bytesPerPixel * (width * height);
	this->textureData->imageData = (GLubyte*)malloc((size_t)imageSize); //Allocate memory

	int bufferSize = (this->textureData->height * this->textureData->width) * this->textureData->bytesPerPixel; // Get the buffer size
	for (int i = 0; i < bufferSize; i += this->textureData->bytesPerPixel) { // For every pixel
		this->textureData->imageData[i] = (GLubyte)255; // Set Red
		this->textureData->imageData[i + 1] = (GLubyte)255; // Set Green
		this->textureData->imageData[i + 2] = (GLubyte)255; // Set Blue

		if (this->textureData->type == GL_RGBA) {
			this->textureData->imageData[i + 3] = (GLubyte)255; // Set alpha
		}
	}

	this->UploadToGPU(); //Re-Upload the texture
}