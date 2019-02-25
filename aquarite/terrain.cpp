// Terrain class uses a heightmap to generate a plane of vertices with heights
//
//	Version: 15/1/2019
//
//	© 2019, Jens Heukers
#include "terrain.h"

Terrain::Terrain() {
	this->model = new Model(); // Create a new model instance
	this->model->IgnoreFrustum(true);
	this->SetModel(model);
}

void Terrain::Construct(Texture* heightMap, Point2f dimensions, int width, int height) {
	//First we want to create a array of all vertices in the terrain, then we want to fill this array
	std::vector<glm::vec3> verts;

	if (heightMap)
		dimensions = Point2f((float)heightMap->textureData->width, (float)heightMap->textureData->height);

	for (int x = 0; x < dimensions.x; x++) for (int z = 0; z < dimensions.y; z++) {
		//Normalize the vertex position to model view
		glm::vec3 pos = glm::vec3(x / dimensions.x, 0, z / dimensions.y);

		//Add the width and height
		pos.x *= width;
		pos.z *= height;

		//Center the model
		pos.x -= width / 2;
		pos.z -= height / 2;

		if (heightMap) {
			int color = heightMap->GetPixelData(x, z, 0); // Get pixel color
			pos.y += color / 255; // Get the normalized color, then add that to height
		}

		//Push back position
		verts.push_back(pos);
	}

	//We want a array of triangles, with this we know what triangle connects with what vertex
	std::vector<Triangle> tris;

	for (size_t i = 0; i < verts.size(); i++) {
		//Get all vertices positions
		glm::vec3 lUp = verts[i];
		glm::vec3 lDown = verts[i + 1];
		glm::vec3 rUp = verts[i + (unsigned)dimensions.y];
		glm::vec3 rDown = verts[(i + (unsigned)dimensions.y) + 1];

		//Now we got all vertices positions, we can construct 2 triangles to make a quad
		//Our normals always face up
		Triangle first;
		first.xCoord = lUp;
		first.xUvCoord = glm::vec2(0, 1);
		first.xNormalCoord = glm::vec3(0, 1, 0);

		first.yCoord = lDown;
		first.yUvCoord = glm::vec2(0, 0);
		first.yNormalCoord = glm::vec3(0, 1, 0);

		first.zCoord = rUp;
		first.zUvCoord = glm::vec2(1, 1);
		first.zNormalCoord = glm::vec3(0, 1, 0);

		Triangle second;
		second.xCoord = rUp;
		second.xUvCoord = glm::vec2(1, 1);
		second.xNormalCoord = glm::vec3(0, 1, 0);

		second.yCoord = rDown;
		second.yUvCoord = glm::vec2(1, 0);
		second.yNormalCoord = glm::vec3(0, 1, 0);

		second.zCoord = lDown;
		second.zUvCoord = glm::vec2(0, 0);
		second.zNormalCoord = glm::vec3(0, 1, 0);

		//Push back the 2 triangles
		tris.push_back(first);
		tris.push_back(second);
	}

	//Now that we have constructed the triangles, we put all the data in one vertex array, then 
	//set the model and material of the terrain.

	std::vector<float> vertices;

	for (size_t i = 0; i < tris.size(); i++) {
		vertices.push_back(tris[i].xCoord.x);
		vertices.push_back(tris[i].xCoord.y);
		vertices.push_back(tris[i].xCoord.z);
		vertices.push_back(tris[i].xUvCoord.x);
		vertices.push_back(tris[i].xUvCoord.y);
		vertices.push_back(tris[i].xNormalCoord.x);
		vertices.push_back(tris[i].xNormalCoord.y);
		vertices.push_back(tris[i].xNormalCoord.z);

		vertices.push_back(tris[i].yCoord.x);
		vertices.push_back(tris[i].yCoord.y);
		vertices.push_back(tris[i].yCoord.z);
		vertices.push_back(tris[i].yUvCoord.x);
		vertices.push_back(tris[i].yUvCoord.y);
		vertices.push_back(tris[i].yNormalCoord.x);
		vertices.push_back(tris[i].yNormalCoord.y);
		vertices.push_back(tris[i].yNormalCoord.z);

		vertices.push_back(tris[i].zCoord.x);
		vertices.push_back(tris[i].zCoord.y);
		vertices.push_back(tris[i].zCoord.z);
		vertices.push_back(tris[i].zUvCoord.x);
		vertices.push_back(tris[i].zUvCoord.y);
		vertices.push_back(tris[i].zNormalCoord.x);
		vertices.push_back(tris[i].zNormalCoord.y);
		vertices.push_back(tris[i].zNormalCoord.z);
	}

	//Create mesh
	Mesh* mesh = new Mesh();
	mesh->GenerateBuffers(vertices);

	//Set propeties
	this->model->AddMesh(mesh);
	this->model->AddMaterial(ResourceManager::GetMaterial("defaultMaterial"));
}

Terrain::~Terrain() {
	delete this->model; // Delete model pointer on destruction
}