/**
*	Filename: model.h
*
*	Description: Header file for Model class.
*
*	Version: 15/1/2019
*
*	© 2018, Jens Heukers
*/
#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "graphics\material.h"
#include "mesh.h"

enum DrawMode {
	Normal = 0,
	Late
};

class Model {
private:
	std::vector<Material*> materials; /// @brief List of materials used on this model, material index should match mesh index
	std::vector<Mesh*> meshes; /// @brief List of meshes used on this Model, material index should match mesh index
	std::string name; /// @brief Name of the model
	float sphereRadius; /// @brief The radius of the sphere of the model, for frustum culling
	bool ignoreFrustum; /// @brief If true frustum culling will be ignored for this model
public:
	DrawMode drawMode; /// @brief specifies if object is to be drawn normally or late

	/**
	* Constructor
	*/
	Model();

	/**
	* Returns the material where index matches
	*/
	Material* GetMaterial(int index);

	/**
	* Sets the material
	*/
	void AddMaterial(Material* material);

	/**
	* Removes a material from the meshes list where index matches
	*/
	void RemoveMaterial(int index);

	/**
	* Returns the amount of materials on this Model
	*/
	int GetMaterialCount();

	/**
	* Returns the mesh where index matches
	*/
	Mesh* GetMesh(int index);

	/**
	* Adds a new mesh to the meshes list
	*/
	void AddMesh(Mesh* material);

	/**
	* Removes a mesh from the meshes list where index matches
	*/
	void RemoveMesh(int index);

	/**
	* Returns the amount of meshes on this Model
	*/
	int GetMeshesCount();

	/**
	* Sets the name of the model
	*/
	void SetName(std::string name);

	/**
	* Returns the name of th emodel
	*/
	std::string GetName();

	/**
	* Returns the sphere radius for this model
	*/
	float GetSphereRadius();

	/**
	* Sets the sphere radius for this model
	*/
	void SetSphereRadius(float amount);

	/**
	* Returns the IgnoreFrustum state
	*/
	bool IgnoreFrustumState();

	/**
	* Sets ignoreFrustum
	*/
	void IgnoreFrustum(bool state);
};
#endif // !MODEL_H