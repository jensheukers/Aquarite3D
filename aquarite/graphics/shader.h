/**
*	Filename: shader.h
*
*	Description: Header file for Shader class.
*
*	Version: 23/12/2018
*
*	© 2018, Jens Heukers
*/
#ifndef SHADER_H
#define SHADER_H
#include "glm/glm.hpp"
#include "GL/glew.h"
#include <string>

class Shader {
private:
	unsigned int _shaderProgram; /// @brief the generated ShaderProgram
	std::string _vertexShaderPath; /// @brief the path to the vertex shader
	std::string _fragmentShaderPath; /// @brief the path to the fragment shader

	/**
	* Reads and returns the shader data
	*/
	std::string LoadShaderFile(std::string shaderPath);

	/**
	* Loads the shader program
	*/
	bool LoadShader(std::string vertexShaderPath, std::string fragmentShaderPath);

public:
	/**
	* Constructor
	*/
	Shader(std::string vertexShaderPath, std::string fragmentShaderPath);

	/**
	* Returns the Shader Program associated with this shader
	*/
	unsigned int GetShaderProgram();

	/**
	* Recompiles the shader program
	*/
	void Recompile();

	//Uniform setters
	void SetBool(std::string uniformName, bool value) const {
		glUniform1i(glGetUniformLocation(_shaderProgram, uniformName.c_str()), (int)value);
	};

	void SetInt(std::string uniformName, int value) const { 
		glUniform1i(glGetUniformLocation(_shaderProgram, uniformName.c_str()), value);
	}

	void SetFloat(std::string uniformName, float value) const { 
		glUniform1f(glGetUniformLocation(_shaderProgram, uniformName.c_str()), value);
	}

	void SetVec2(std::string uniformName, glm::vec2& value) const {
		glUniform2f(glGetUniformLocation(_shaderProgram, uniformName.c_str()), value[0], value[1]);
	}

	void SetVec3(std::string uniformName, glm::vec3& value) const {
		glUniform3f(glGetUniformLocation(_shaderProgram, uniformName.c_str()), value[0], value[1], value[2]);
	}

	void SetVec4(std::string uniformName, glm::vec4& value) const {
		glUniform4f(glGetUniformLocation(_shaderProgram, uniformName.c_str()), value[0], value[1], value[2], value[3]);
	}

	void SetMat2(std::string uniformName, glm::mat2 value) const { 
		glUniformMatrix2fv(glGetUniformLocation(_shaderProgram, uniformName.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	void SetMat3(std::string uniformName, glm::mat3 value) const {
		glUniformMatrix3fv(glGetUniformLocation(_shaderProgram, uniformName.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	void SetMat4(std::string uniformName, glm::mat4 value) const {
		glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, uniformName.c_str()), 1, GL_FALSE, &value[0][0]);
	}
};

#endif // !SHADER_H
