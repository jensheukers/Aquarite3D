/**
*	Filename: mesh.cpp
*
*	Description: Source file for Mesh class.
*
*	Version: 20/12/2018
*
*	© 2018, Jens Heukers
*/
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "mesh.h"
#include "debug.h"

Mesh::Mesh() {
	this->_verticesCount = 0;
	this->_vao = NULL;
	this->_vbo = NULL;
}

GLuint Mesh::GetVBO() {
	return this->_vbo;
}

GLuint Mesh::GetVAO() {
	return this->_vao;
}

unsigned Mesh::GetVerticesCount() {
	return this->_verticesCount;
}

void Mesh::LoadObj(std::string path) {
	// Create Temporary arrays
	std::vector<glm::vec3> _vertices; // vector containing all vertices
	std::vector<glm::vec2> _uvs; // vector containing all uv coordinates
	std::vector<glm::vec3> _normals; // vector containing all normals
	std::vector<Triangle> _triangles; // vector containing triangles


	//Read the file and parse data to temporary arrays
	std::string line;
	std::ifstream file(path);
	
	if (file.is_open()) {
		while (getline(file, line)) {
			std::stringstream ss(line);
			std::string segment;
			std::vector<std::string> segments;
			while (std::getline(ss, segment, ' ')) { // Split by space character
				segments.push_back(segment);
			}

			// If Vertex
			if (segments[0] == "v") {
				_vertices.push_back(glm::vec3(std::stof(segments[1]), std::stof(segments[2]), std::stof(segments[3]))); //Push back
			}

			// If UV Coordinate
			if (segments[0] == "vt") {
				_uvs.push_back(glm::vec2(std::stof(segments[1]),std::stof(segments[2]))); // Push back
			}

			// If Normal
			if (segments[0] == "vn") {
				_normals.push_back(glm::vec3(std::stof(segments[1]), std::stof(segments[2]), std::stof(segments[3]))); //Push back
			}

			// If face
			if (segments[0] == "f") {
				Triangle triangle; // The triangle where data is to be stored in

				std::vector<std::string> _xtriangleInfo;
				std::vector<std::string> _ytriangleInfo;
				std::vector<std::string> _ztriangleInfo;

				std::stringstream ss(segments[1]);
				std::string segment;
				while (std::getline(ss, segment, '/')) { // Split by space character
					_xtriangleInfo.push_back(segment);
				}

				std::stringstream ss1(segments[2]);
				while (std::getline(ss1, segment, '/')) { // Split by space character
					_ytriangleInfo.push_back(segment);
				}

				std::stringstream ss2(segments[3]);
				while (std::getline(ss2, segment, '/')) { // Split by space character
					_ztriangleInfo.push_back(segment);
				}

				triangle.xCoord = _vertices[std::stoi(_xtriangleInfo[0]) - 1];
				triangle.xUvCoord = _uvs[std::stoi(_xtriangleInfo[1]) - 1];
				triangle.xNormalCoord = _normals[std::stoi(_xtriangleInfo[2]) - 1];

				triangle.yCoord = _vertices[std::stoi(_ytriangleInfo[0]) - 1];
				triangle.yUvCoord = _uvs[std::stoi(_ytriangleInfo[1]) - 1];
				triangle.yNormalCoord = _normals[std::stoi(_ytriangleInfo[2]) - 1];

				triangle.zCoord = _vertices[std::stoi(_ztriangleInfo[0]) - 1];
				triangle.zUvCoord = _uvs[std::stoi(_ztriangleInfo[1]) - 1];
				triangle.zNormalCoord = _normals[std::stoi(_ztriangleInfo[2]) - 1];

				_triangles.push_back(triangle);
			}
		}
		file.close();
	}
	
	std::vector<float> _finalVertices;

	for (size_t i = 0; i < _triangles.size(); i++) {
		_finalVertices.push_back(_triangles[i].xCoord.x);
		_finalVertices.push_back(_triangles[i].xCoord.y);
		_finalVertices.push_back(_triangles[i].xCoord.z);
		_finalVertices.push_back(_triangles[i].xUvCoord.x);
		_finalVertices.push_back(_triangles[i].xUvCoord.y);
		_finalVertices.push_back(_triangles[i].xNormalCoord.x);
		_finalVertices.push_back(_triangles[i].xNormalCoord.y);
		_finalVertices.push_back(_triangles[i].xNormalCoord.z);

		_finalVertices.push_back(_triangles[i].yCoord.x);
		_finalVertices.push_back(_triangles[i].yCoord.y);
		_finalVertices.push_back(_triangles[i].yCoord.z);
		_finalVertices.push_back(_triangles[i].yUvCoord.x);
		_finalVertices.push_back(_triangles[i].yUvCoord.y);
		_finalVertices.push_back(_triangles[i].yNormalCoord.x);
		_finalVertices.push_back(_triangles[i].yNormalCoord.y);
		_finalVertices.push_back(_triangles[i].yNormalCoord.z);

		_finalVertices.push_back(_triangles[i].zCoord.x);
		_finalVertices.push_back(_triangles[i].zCoord.y);
		_finalVertices.push_back(_triangles[i].zCoord.z);
		_finalVertices.push_back(_triangles[i].zUvCoord.x);
		_finalVertices.push_back(_triangles[i].zUvCoord.y);
		_finalVertices.push_back(_triangles[i].zNormalCoord.x);
		_finalVertices.push_back(_triangles[i].zNormalCoord.y);
		_finalVertices.push_back(_triangles[i].zNormalCoord.z);
	}

	GenerateBuffers(_finalVertices);
}

void Mesh::GenerateBuffers(std::vector<float>& vertices) {
	// Generate VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Generate Buffers
	glGenBuffers(1, &_vbo);

	//Copy vertices to VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	//Handle VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Set Vertices Count
	this->_verticesCount = vertices.size();
}

Mesh::~Mesh() {
	if (this->_vao != NULL) {
		glDeleteVertexArrays(1, &_vao);
	}

	if (this->_vbo != NULL) {
		glDeleteBuffers(1, &_vbo);
	}
}