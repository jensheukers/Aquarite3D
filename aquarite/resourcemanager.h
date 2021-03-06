/**
*	Filename: resourcemanager.h
*
*	Description: Header file for ResourceManager singleton class.
*
*	Version: 2/28/2019
*
*	� 2019, Jens Heukers
*/
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <vector>
#include <map>

struct Meta {
	std::string resourceName;
	std::string offset;
	std::string author;
	float version;
};

//Forward Declarations
class Texture;
class Mesh;
class Shader;
class Material;
class Model;

class ResourceManager {
private:
	static ResourceManager* _instance; /// @brief Resource Manager singleton instance
	std::map<std::string, Texture*> _textures; /// @brief Map containing all textures
	std::map<std::string, Mesh*> _meshes; /// @brief Map containing all meshes
	std::map<std::string, Shader*> _shaders; /// @brief Map containing all shaders
	std::map<std::string, Material*> _materials; /// @brief Map containing all shaders
	std::map<std::string, Model*> _models; /// @brief Map containing all models
public:
	/**
	* Returns the instance
	*/
	static ResourceManager* GetInstance();

	/**
	* Adds a new texture to the textures map
	*/
	static void AddTexture(std::string key, Texture* texture);

	/**
	* Retrieves a texture from the textures map
	*/
	static Texture* GetTexture(std::string key);

	/**
	* Deletes a texture from the textures map
	*/
	static void RemoveTexture(std::string key);

	/**
	* Returns a texture key name
	*/
	static std::string GetTextureKeyName(Texture* texture);

	/**
	* Adds a new mesh to the meshes map
	*/
	static void AddMesh(std::string key, Mesh* mesh);

	/**
	* Retrieves a mesh from the meshes map
	*/
	static Mesh* GetMesh(std::string key);

	/**
	* Deletes a mesh from the meshes map
	*/
	static void RemoveMesh(std::string key);

	/**
	* Adds a new shader to the shaders map
	*/
	static void AddShader(std::string key, Shader* shader);

	/**
	* Retrieves a shader from the shaders map
	*/
	static Shader* GetShader(std::string key);

	/**
	* Deletes a shader from the shaders map
	*/
	static void RemoveShader(std::string key);

	/**
	* Adds a new material to the materials map
	*/
	static void AddMaterial(std::string key, Material* material);

	/**
	* Retrieves a material from the materials map
	*/
	static Material* GetMaterial(std::string key);

	/**
	* Deletes a material from the materials map
	*/
	static void RemoveMaterial(std::string key);

	/**
	* Adds a new model to the materials map
	*/
	static void AddModel(std::string key, Model* material);

	/**
	* Retrieves a model from the materials map
	*/
	static Model* GetModel(std::string key);

	/**
	* Deletes a model from the models map
	*/
	static void RemoveModel(std::string key);

	/**
	* Loads external meta file and loads specified resources into memory
	*/
	static void LoadMeta(std::string offset);

	/**
	* Unloads all resources from the heap.
	*/
	static void UnLoad();

	/**
	* Destructor, Unloads all assets and deletes lists
	*/
	~ResourceManager();
};


#endif // !RESOURCEMANAGER_H

