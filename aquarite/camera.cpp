/**
*	Filename: camera.cpp
*
*	Description: Source file for Camera and Frustum class.
*
*	Version: 13/1/2019
*
*	© 2018, Jens Heukers
*/
#include <iostream>
#include "camera.h"
#include "input.h"

Plane::Plane(Vec3 &v1, Vec3 &v2, Vec3 &v3) {
	set3Points(v1, v2, v3);
}

Plane::Plane() {}

Plane::~Plane() {}


void Plane::set3Points(Vec3 &v1, Vec3 &v2, Vec3 &v3) {
	Vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = aux2 * aux1;

	normal.Normalize();
	point = Vec3(v2.x, v2.y, v2.z);
	d = -(normal.InnerProduct(point));
}

void Plane::setNormalAndPoint(Vec3 &normal, Vec3 &point) {

	this->normal = Vec3(normal.x, normal.y, normal.z);
	this->normal.Normalize();
	d = -(this->normal.InnerProduct(point));
}

void Plane::setCoefficients(float a, float b, float c, float d) {

	// set the normal vector
	normal = Vec3(a, b, c);
	//compute the lenght of the vector
	float l = normal.Magnitude();
	// normalize the vector
	normal = Vec3(a / l, b / l, c / l);
	// and divide d by th length as well
	this->d = d / l;
}

float Plane::distance(Vec3 &p) {

	return (d + normal.InnerProduct(p));
}

//Frustum class
#define ANG2RAD 3.14159265358979323846/180.0

Frustum::Frustum() {}

Frustum::~Frustum() {}

void Frustum::setCamInternals(float angle, float ratio, float nearD, float farD) {
	// store the information
	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	// compute width and height of the near and far plane sections
	tang = (float)tan(ANG2RAD * angle * 0.5);
	nh = nearD * tang;
	nw = nh * ratio;
	fh = farD * tang;
	fw = fh * ratio;
}

void Frustum::setCamDef(Vec3 &p, Vec3 &l, Vec3 &u) {

	Vec3 dir, nc, fc, X, Y, Z;

	// compute the Z axis of camera
	// this axis points in the opposite direction from 
	// the looking direction
	Z = p - l;
	Z.Normalize();

	// X axis of camera with given "up" vector and Z axis
	X = u * Z;
	X.Normalize();

	// the real "up" vector is the cross product of Z and X
	Y = Z * X;

	// compute the centers of the near and far planes
	nc = p - Z * nearD;
	fc = p - Z * farD;

	// compute the 4 corners of the frustum on the near plane
	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	// compute the 4 corners of the frustum on the far plane
	ftl = fc + Y * fh - X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;

	// compute the six planes
	// the function set3Points assumes that the points
	// are given in counter clockwise order
	pl[TOP].set3Points(ntr, ntl, ftl);
	pl[BOTTOM].set3Points(nbl, nbr, fbr);
	pl[LEFT].set3Points(ntl, nbl, fbl);
	pl[RIGHT].set3Points(nbr, ntr, fbr);
	pl[NEARP].set3Points(ntl, ntr, nbr);
	pl[FARP].set3Points(ftr, ftl, fbl);
}

int Frustum::pointInFrustum(Vec3 &p) {
	int result = INSIDE;
	for (int i = 0; i < 6; i++) {

		if (pl[i].distance(p) < 0)
			return OUTSIDE;
	}
	return(result);
}

int Frustum::sphereInFrustum(Vec3 &p, float radius) {
	float distance;
	int result = INSIDE;

	for (int i = 0; i < 6; i++) {
		distance = pl[i].distance(p);
		if (distance < -radius)
			return OUTSIDE;
		else if (distance < radius)
			result = INTERSECT;
	}
	return(result);
}


Camera::Camera() {
	this->frustum = new Frustum();
	this->target = glm::vec3(0.0f, 0.0f, -1.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
}

Frustum* Camera::GetFrustum() {
	return this->frustum;
}

void Camera::UpdateFront() {
	if (yaw > 360) {
		yaw = 0;
	}
	
	if (yaw < 0) {
		yaw = 360;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	this->SetTarget(glm::normalize(front));
}

void Camera::SetYaw(float amount) {
	this->yaw = amount;
}

float Camera::GetYaw() {
	return this->yaw;
}

void Camera::SetPitch(float amount) {
	this->pitch = amount;
}

float Camera::GetPitch() {
	return this->pitch;
}

void Camera::SetPos(glm::vec3 pos) {
	this->pos = pos;
}

glm::vec3 Camera::GetPos() {
	return this->pos;
}

void Camera::SetTarget(glm::vec3 target) {
	this->target = target;
}

glm::vec3 Camera::GetTarget() {
	return this->target;
}

void Camera::SetUp(glm::vec3 up) {
	this->up = up;
}

glm::vec3 Camera::GetUp() {
	return this->up;
}

FPSCamera::FPSCamera() {
	firstMouse = true;
	sensitivity = 0.1f;
	yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	pitch = 0.0f;
	lastX = 800.0f / 2.0;
	lastY = 600.0 / 2.0;
	fov = 45.0f;
}

void FPSCamera::Move(CameraDirection direction, float speed) {
	if (direction == CameraDirection::Forward) {
		this->pos += speed * this->target;
	}
	if (direction == CameraDirection::Backward) {
		this->pos -= speed * this->target;
	}
	if (direction == CameraDirection::Left) {
		this->pos -= glm::normalize(glm::cross(this->target, this->up)) * speed;
	}
	if (direction == CameraDirection::Right) {
		this->pos += glm::normalize(glm::cross(this->target, this->up)) * speed;
	}
}

void FPSCamera::OnMouseMovement(float xpos, float ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}

void FPSCamera::SetSensitivity(float amount) {
	this->sensitivity = amount;
}

float FPSCamera::GetSensitivity() {
	return this->sensitivity;
}