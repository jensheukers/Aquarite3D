/**
*	Filename: camera.h
*
*	Description: Header file for Camera class.
*
*	Version: 8/2/2019
*
*	© 2018, Jens Heukers
*/
#ifndef CAMERA_H
#define CAMERA_H 
#include <glm/glm.hpp>
#include "math/vec3.h"
#include "math/pointx.h"

/*
	Frustum  class copied from: http://cgvr.informatik.uni-bremen.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html
	Implementation is not the best there is but it will do the job :)
*/

class Plane {
public:
	Vec3 normal, point;
	float d;

	Plane::Plane(Vec3 &v1, Vec3 &v2, Vec3 &v3);
	Plane::Plane(void);
	Plane::~Plane();

	void set3Points(Vec3 &v1, Vec3 &v2, Vec3 &v3);
	void setNormalAndPoint(Vec3 &normal, Vec3 &point);
	void setCoefficients(float a, float b, float c, float d);
	float distance(Vec3 &p);
};

class Frustum {
private:
	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
	};

public:

	enum { OUTSIDE, INTERSECT, INSIDE };

	Plane pl[6];

	Vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	float nearD, farD, ratio, angle, tang;
	float nw, nh, fw, fh;

	Frustum::Frustum();
	Frustum::~Frustum();

	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(Vec3 &p, Vec3 &l, Vec3 &u);
	int pointInFrustum(Vec3 &p);
	int sphereInFrustum(Vec3 &p, float raio);
};

//---------------------------------------------------
//Camera class
//--------------------------------------------------

class Camera {
protected:
	Frustum * frustum; /// @brief The frustum of the camera

	float yaw; /// @brief the yaw of the camera
	float pitch; /// @brief the pitch of the camera

	glm::vec3 pos; /// @brief The camera position in world space
	glm::vec3 target; /// @brief The camera target
	glm::vec3 up; /// @brief The camera up vector
public:
	bool lookAtTarget; /// @brief If true, camera will not update target but set focus to it

	/**
	* Constructor
	*/
	Camera();

	/**
	* Returns the camera frustum
	*/
	Frustum* GetFrustum();

	/**
	* Updates the camera's front
	*/
	void UpdateFront();

	/**
	* Set the Yaw of the camera
	*/
	void SetYaw(float amount);

	/**
	* Gets the Yaw of the camera
	*/
	float GetYaw();

	/**
	* Set the Pitch of the camera
	*/
	void SetPitch(float amount);

	/**
	* Gets the Pitch of the camera
	*/
	float GetPitch();

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
