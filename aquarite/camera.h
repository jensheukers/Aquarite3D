/**
*	Filename: camera.h
*
*	Description: Header file for Camera class.
*
*	Version: 10/1/2019
*
*	© 2018, Jens Heukers
*/
#ifndef CAMERA_H
#define CAMERA_H 
#include <glm/glm.hpp>
#include "math/vec3.h"
#include "math/pointx.h"

class Camera {
protected:
	glm::vec3 pos; /// @brief The camera position in world space
	glm::vec3 target; /// @brief The camera target
	glm::vec3 up; /// @brief The camera up vector
public:
	Camera();

	/**
	* Set the position of the camera
	*/
	void SetPos(glm::vec3 pos);

	/**
	* Get the position of the camera
	*/
	glm::vec3 GetPos();

	/**
	* Set the front of the camera (direction)
	*/
	void SetTarget(glm::vec3 targetPos);

	/**
	* Returns the front of the camera (direction)
	*/
	glm::vec3 GetTarget();

	/**
	* Set the up of the camera
	*/
	void SetUp(glm::vec3 up);

	/**
	* Returns the up of the camera
	*/
	glm::vec3 GetUp();
};

//class is copied from https://learnopengl.com/Getting-started/Camera
//class can be used for simple FPS-Camera

enum CameraDirection {
	Forward,
	Backward,
	Left,
	Right
};

class FPSCamera : public Camera {
private:
	float yaw;	
	float pitch;
	float lastX;
	float lastY;
	float fov;

	float sensitivity; /// @brief The sensitivity of the camera
public:
	bool firstMouse;
	/**
	* Constructor
	*/
	FPSCamera();

	/**
	* Move the camera is certain direction
	*/
	void Move(CameraDirection direction, float speed);

	/**
	* Method should be called when mouse position changes
	*/
	void OnMouseMovement(float xpos, float ypos);

	/**
	* Sets the sensitivity
	*/
	void SetSensitivity(float amount);

	/**
	* Gets the sensitivity
	*/
	float GetSensitivity();
};

#endif // !CAMERA_H
