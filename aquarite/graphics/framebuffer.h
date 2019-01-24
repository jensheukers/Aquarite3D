/**
*	Filename: framebuffer.h
*
*	Description: Header file for FrameBuffer class.
*
*	Version: 15/1/2019
*
*	© 2018, Jens Heukers
*/

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "../math/pointx.h"
#include "shader.h"

class FrameBuffer {
private:
	unsigned int fbo; /// @brief The frame buffer object
	unsigned int rbo; /// @brief The render buffer object
	unsigned int textureColorBuffer; /// @brief The texture color buffer object
	Shader* shader; /// @brief Pointer to the frame buffer shader
public:
	/**
	* Constructor
	*/
	FrameBuffer(Point2i size, GLenum attachment);

	/**
	* returns the frame buffer object
	*/
	unsigned int GetFBO();

	/**
	* returns the render buffer object
	*/
	unsigned int GetRBO();

	/**
	* returns the texture color buffer object
	*/
	unsigned int GetTextureColorBufferObject();

	/**
	* Returns the shader of the frame buffer
	*/
	Shader* GetShader();

	/**
	* Sets the shader of the frame buffer
	*/
	void SetShader(Shader* shader);
};

#endif // !FRAMEBUFFER_H