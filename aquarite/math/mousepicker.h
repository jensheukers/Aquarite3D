// Handles transformations for mouse from view space to world space,
// Allowing for ray picking. This is put into a seperate class due to more simplicity and clarity
// Version: 1/31/2019
// © 2019, Jens Heukers
#ifndef MOUSEPICKER_H
#define MOUSEPICKER_H
#include "glm/glm.hpp"
#include "vec3.h"
#include "../camera.h"

/**
* Mousepicker handles transformations for mouse from view space to world space.
*/
class MousePicker {
private:
	glm::vec3 currentRay; /**< The current ray */

	/**
	* Calculates the mouse ray
	*/
	glm::vec3 CalculateMouseRay();

	/**
	* Returns the normalized device coordinates
	*/
	glm::vec2 GetNormalizedDeviceCoordinates(float mouseX, float mouseY);

	/**
	* Returns the eye coords vector4
	*/
	glm::vec4 ToEyeCoords(glm::vec4 clipCoords);

	/**
	* Translates from eye coordinates to world coordinates
	*/
	glm::vec3 ToWorldCoords(glm::vec4 eyeCoords);
public:
	/**
	* Update method, is to be called every frame
	* @param Camera pointer.
	* @param glm::mat4 Projection Matrix
	* @return void
	*/
	void Update();

	/**
	* Returns the current ray
	* @return Vec3
	*/
	Vec3 GetCurrentRay();
};

#endif // !MOUSEPICKER_H
