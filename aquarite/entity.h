/**
*	Filename: entity.h
*
*	Description: Header file for Entity class, also contains definitions for Sprite (basicly a quad)
*
*	Version: 28/12/2018
*
*	© 2018, Jens Heukers
*/

#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <glm/glm.hpp>
#include "renderer.h"
#include "math/vec3.h"
#include "model.h"

// Enum with available rendering types
enum RenderMode {
	WorldSpace, /// @brief WorldSpace rendering is for entities that are to be rendered in the world
	ScreenSpace /// @brief ScreenSpace rendering is mainly used for 2D Sprite rendering
};

class Entity {
private:
	//Global members
	static unsigned _currentId; /// @brief Global current id, increments each time a new entity is instanciated.
	std::string name; /// @brief The name of the Entity.

	//Local members
	unsigned id; /// @brief The Id of this entity
	RenderMode renderMode; /// @brief Type of rendering to be applied to this entity

	Vec3 globalPosition; /// @brief the exact Position in world space.
	Vec3 localRotation; /// @brief local rotation Vector3
	Vec3 globalRotation; /// @brief global rotation Vector3, relative to parent
	Vec3 localScale; /// @brief Local Scale Vector3
	Vec3 globalScale; /// @brief Global Scale Vector3, relative to parent scaling
	
	std::vector<Entity*> children; /// @brief Vector of children Entities
	Entity* parent; /// @brief The parent entity of this entity, if entity has no parent will be set to nullptr.

	Model* model; /// @brief The model that this entity uses
protected:
	/**
	* Protected method UpdateChildren, this due to we not wanting the end user to call this method.
	*/
	void UpdateChildren();

	/**
	* Protected method Render, this due to we not wanting the end user to call this method.
	*/
	void Render(Renderer* renderer, Camera* camera);
public:
	Vec3 position; /// @brief Position Vector3, position is relative to the parent's position

	/**
	* Constructor, if RenderMode is not defined then RenderMode will be WorldSpace
	*/
	Entity(RenderMode renderType = RenderMode::WorldSpace, Shader* shader = nullptr, Material* = nullptr);

	/**
	* Returns id of the entity
	*/
	unsigned GetId();

	/**
	* Set the name of the entity
	*/
	void SetName(std::string name);

	/**
	* Get the name of the entity
	*/
	std::string GetName();

	/**
	* Update gets called every frame
	*/
	virtual void Update() {};

	/**
	* Returns the child where index matches
	*/
	Entity* GetChild(int index);

	/**
	* Adds a new child to the children vector, then returns the child, also sets the child's parent to this object
	*/
	Entity* AddChild(Entity* entity);

	/**
	* Removes a child from the children vector
	*/
	void RemoveChild(Entity* entity);

	/**
	* Returns the vector of children
	*/
	std::vector<Entity*> GetChildren();

	/**
	* Add given Vec3 to position
	*/
	void Translate(Vec3 amount);

	/**
	* Get the global rotation of the entity, note that this position cannot and should not be modified!
	*/
	Vec3 GetPositionGlobal();

	/**
	* Sets the local rotation of the entity
	*/
	void SetRotation(Vec3 rotation);

	/**
	* Adds Given Vec3 to rotation
	*/
	void Rotate(Vec3 amount);

	/**
	* Get the local rotation of the entity
 	*/
	Vec3 GetRotation();

	/**
	* Get the global rotation of the entity
	*/
	Vec3 GetRotationGlobal();

	/**
	* Set the local scale of this Entity
	*/
	void SetScale(Vec3 scale);

	/**
	* Get the local scale of the entity
	*/
	Vec3 GetScale();

	/**
	* Get the global scale of the entity
	*/
	Vec3 GetScaleGlobal();

	/**
	* Sets the model of the entity
	*/
	void SetModel(Model* model);

	/**
	* Returns the model of the entity
	*/
	Model* GetModel();

	/**
	* Sets the RenderMode
	*/
	void SetRenderMode(RenderMode renderMode);

	/**
	* Returns the RenderMode
	*/
	RenderMode GetRenderMode();

	/**
	* Sets the parent object of this entity if parent is not nullptr
	*/
	void SetParent(Entity* parent);

	/**
	* Returns the parent entity of this object
	*/
	Entity* GetParent();

	/**
	* Destructor
	*/
	~Entity();
};

/**
* UV Data structure containing 4 glm::vec2 points defining uv coordinates
*/
struct UVData4p {
	glm::vec2 _leftUp; /// @brief The Left Up Coordinate
	glm::vec2 _rightUp; /// @brief The Right Up Coordinate
	glm::vec2 _leftDown; /// @brief The Left Down Coordinate
	glm::vec2 _rightDown; /// @brief The Right Down Coordinate
};

class Sprite : public Entity {
private:
	UVData4p _uvData; /// @brief 4 point UV Data structure holding uv data

	/**
	* Create the sprite, can only be called from within
	*/
	void Create();
public:
	// Default UV Coords public due to we want to provide the end user with this data if requested
	static float _defaultUvCoords[8]; /// @brief Array Containing Default UV Coords

	/**
	* Default Constructor, takes in 4 parameters defining uv coordinates and a parameter defining the Rendering Mode
	*/
	Sprite(float _uvCoords[8] = _defaultUvCoords, RenderMode renderType = RenderMode::ScreenSpace);

	/**
	* Constructor taking a _uvData structure
	*/
	Sprite(UVData4p _uvData);
};

#endif // !ENTITY_H