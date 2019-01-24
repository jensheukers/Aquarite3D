/**
*	Filename: framebuffer.cpp
*
*	Description: Source file for FrameBuffer class.
*
*	Version: 15/1/2019
*
*	© 2018, Jens Heukers
*/
#include <GL/glew.h>
#include "../debug.h"
#include "framebuffer.h"

FrameBuffer::FrameBuffer(Point2i size, GLenum attachment) {
	//Set shader to nullptr
	this->shader = nullptr;

	//Generate buffers
	glGenFramebuffers(1, &fbo); // Generate the frame buffer object
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//Texture color buffer object
	glGenTextures(1, &textureColorBuffer); // Generate texture object
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureColorBuffer, 0);

	//Render buffer object
	glGenRenderbuffers(1, &rbo); // Generate render buffor object
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); 

	//Check if success
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Debug::Log("Error: Framebuffer is not complete!", typeid(*this).name());
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind
}

unsigned int FrameBuffer::GetFBO() {
	return this->fbo;
}

unsigned int FrameBuffer::GetRBO() {
	return this->rbo;
}

unsigned int FrameBuffer::GetTextureColorBufferObject() {
	return this->textureColorBuffer;
}

Shader* FrameBuffer::GetShader() {
	return this->shader;
}

void FrameBuffer::SetShader(Shader* shader) {
	this->shader = shader;
}