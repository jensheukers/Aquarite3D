/**
*	Filename: shader.cpp
*
*	Description: Source file for Shader class.
*
*	Version: 23/12/2018
*
*	© 2018, Jens Heukers
*/
#include "shader.h"
#include "../debug.h"
#include "../core.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string Shader::LoadShaderFile(std::string shaderPath) {
	std::string buildPath = Core::GetBuildDirectory();
	std::ifstream _file;
	_file.open(buildPath.append(shaderPath)); // Open the file

	if (!_file) { // If we cannot open
		Debug::Log("Cannot open vertex or fragment shader file, returning empty string", typeid(*this).name()); // Log error
 		return ""; // Return emptry string
	}

	std::stringstream stream; // Create a new string stream
	stream << _file.rdbuf(); // Read the file buffer
	_file.close(); // Close the file

	return stream.str(); // Return the stream string
}

bool Shader::LoadShader(std::string vertexShaderPath, std::string fragmentShaderPath) {
	std::string _vertexData = this->LoadShaderFile(vertexShaderPath); // Load the vertex shader file
	std::string _fragmentData = this->LoadShaderFile(fragmentShaderPath); // Load the fragment shader file

	if (_vertexData == "" || _fragmentData == "") {
		return 1;
	}

	const char* _vertexDataCStr = _vertexData.c_str(); // Convert to C Style String
	const char* _fragmentDataCStr = _fragmentData.c_str(); // Convert to C Style String

														   //Vertex Shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &_vertexDataCStr, NULL);
	glCompileShader(vertexShader);

	int  vsuccess;
	char vinfoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vsuccess);
	if (!vsuccess)
	{
		Debug::Log("Vertex shader could not be compiled!", typeid(*this).name());
		Debug::Log(vinfoLog, typeid(*this).name());
		return 1;
	}

	//Fragment Shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &_fragmentDataCStr, NULL);
	glCompileShader(fragmentShader);

	int  fsuccess;
	char finfoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fsuccess);
	if (!fsuccess)
	{
		Debug::Log("Fragment shader could not be compiled!", typeid(*this).name());
		Debug::Log(finfoLog, typeid(*this).name());
		return 1;
	}

	//Create the shader program
	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);
	glLinkProgram(_shaderProgram);

	// print linking errors if any
	int  psuccess;
	char pinfoLog[512];
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &psuccess);
	if (!psuccess)
	{
		glGetProgramInfoLog(_shaderProgram, 512, NULL, pinfoLog);
		Debug::Log("Shader program could not be created!", typeid(*this).name());
		Debug::Log(pinfoLog, typeid(*this).name());
		return 1;
	}

	this->_vertexShaderPath = vertexShader;
	this->_fragmentShaderPath = fragmentShaderPath;

	//Cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return 0;
}

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
	if (this->LoadShader(vertexShaderPath, fragmentShaderPath) != 0) {
		Debug::Log("Failed to create shader program...", typeid(*this).name());
		return;
	}

	Debug::Log("Succesfully created shader program", typeid(*this).name());
}

void Shader::Recompile() {
	if (!this->_shaderProgram) {
		Debug::Log("No shader program, cant recompile", typeid(*this).name());
	}

	glDeleteProgram(this->_shaderProgram); // Delete shader program
	
	if (this->LoadShader(this->_vertexShaderPath, this->_fragmentShaderPath) == 0) {
		Debug::Log("Failed to recompile Shader Program", typeid(*this).name());
	}
}

unsigned int Shader::GetShaderProgram() {
	return this->_shaderProgram;
}