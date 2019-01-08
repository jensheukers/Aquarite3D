/**
*	Filename: mesh.h
*
*	Description: Header file for Mesh class.
*
*	Version: 20/12/2018
*
*	© 2018, Jens Heukers
*/
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <GL/glew.h>
#include "glm/glm.hpp"

struct Triangle {
	glm::vec3 xCoord; /// @ brief The First verte3x of the Triangle.
	glm::vec2 xUvCoord; /// @ brief The First Uv Coordinate of the Triangle. 
	glm::vec3 xNormalCoord; /// @brief The First Normal Coordinate of the Triangle.
	glm::vec3 yCoord; /// @ brief The Second vertex of the Triangle.
	glm::vec2 yUvCoord; /// @ brief The Second Uv Coordinate of the Triangle. 
	glm::vec3 yNormalCoord; /// @brief The second Normal Coordinate of the Triangle.
	glm::vec3 zCoord; /// @ brief The Third vertex of the Triangle.
	glm::vec2 zUvCoord; /// @ brief The Third Uv Coordinate of the Triangle. 
	glm::vec3 zNormalCoord; /// @brief The Third Normal Coordinate of the Triangle.
};

class Mesh {
private:
	GLuint _vbo; /// @brief The Vertex Buffer Object
	GLuint _vao; /// @brief The Vertex Array Object
	unsigned _verticesCount; /// @brief Amount of vertices that this object contains
public:
	Mesh();

	/**
	* Returns the Vertex Buffer Object
	*/
	GLuint GetVBO();

	/**
	* Returns the Vertex Array Object
	*/
	GLuint GetVAO();

	/**
	* Returns the amount of vertices in this mesh
	*/
	unsigned GetVerticesCount();

	/**
	* Loads a obj file and generates buffers
	*/
	void LoadObj(std::string path);

	/**
	* Generate a buffer for given vertices
	*/
	void GenerateBuffers(std::vector<float>& vertices);

	/**
	* Destructor
	*/
	~Mesh();
};
#endif // !MESH_H