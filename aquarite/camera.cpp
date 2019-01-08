/**
*	Filename: camera.cpp
*
*	Description: Source file for Camera class.
*
*	Version: 30/12/2018
*
*	© 2018, Jens Heukers
*/
#include <iostream>
#include "camera.h"
#include "input.h"

Camera::Camera() {
	this->target = glm::vec3(0.0f, 0.0f, -1.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
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

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->target = glm::normalize(front);
}

void FPSCamera::SetSensitivity(float amount) {
	this->sensitivity = amount;
}

float FPSCamera::GetSensitivity() {
	return this->sensitivity;
}