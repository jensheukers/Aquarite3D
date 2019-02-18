// Terrain class uses a heightmap to generate a plane of vertices with heights
//
//	Version: 15/1/2019
//
//	© 2019, Jens Heukers
#ifndef TERRAIN_H
#define TERRAIN_H
#include "entity.h"

/**
* Terrain class extends from entity, whenever a terrain is constructed, a model will be created,
* Then that model will be attached to the Entity
*/
class Terrain : public Entity {
private:
	Model* model; /**< The model of the entity.*/
public:
	/**
	* Constructor
	*/
	Terrain();

	/**
	* Constructs the terrain, Note that when a heightmap is passed, dimensions will be set to texture data width and height
	* @param heightMap the heightmap the terrain uses
	* @param dimensions Point2f defining the resolution of the terrain
	* @param width The width the terrain should be.
	* @param height The height the terrain should be.
	* @return void
	*/
	void Construct(Texture* heightMap = nullptr, Point2f dimensions = Point2f(128, 128), int width = 10, int height = 10);

	/**
	* Destructor
	*/
	~Terrain();
};

#endif // !TERRAIN_H