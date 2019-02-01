// Source implementation for mousepicker.h
// Version: 1/31/2019
// © 2019, Jens Heukers
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include "../core.h"
#include "../input.h"
#include "mousepicker.h"

glm::vec3 MousePicker::CalculateMouseRay() {
	float mouseX = Input::GetMousePosition().x;
	float mouseY = Input::GetMousePosition().y;
	glm::vec2 normalizedCoords = GetNormalizedDeviceCoordinates(mouseX, mouseY);
	glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.f, 1.f);
	glm::vec4 eyeCoords = ToEyeCoords(clipCoords);
	return ToWorldCoords(eyeCoords);
}

glm::vec2 MousePicker::GetNormalizedDeviceCoordinates(float mouseX, float mouseY) {
	float x = (2.f * mouseX) / Core::GetResolution().x - 1;
	float y = (2.f * mouseY) / Core::GetResolution().y - 1.f;
	return glm::vec2(x, -y);
}

glm::vec4 MousePicker::ToEyeCoords(glm::vec4 clipCoords) {
	glm::mat4 invertedProjection = glm::inverse(Core::GetRendererProjectionMatrix());
	glm::vec4 eyeCoords = invertedProjection * clipCoords;
	return glm::vec4(eyeCoords.x, eyeCoords.y, -1, 1);
}

glm::vec3 MousePicker::ToWorldCoords(glm::vec4 eyeCoords) {
	glm::mat4 invertedView = glm::inverse(Core::GetRendererViewMatrix());
	glm::vec4 rayWorld = invertedView * eyeCoords;
	glm::vec3 mouseRay = glm::vec3(rayWorld);
	glm::normalize(mouseRay);
	return mouseRay;
}

void MousePicker::Update() {
	currentRay = CalculateMouseRay();
}

Vec3 MousePicker::GetCurrentRay() {
	return Vec3::ToVec3(currentRay);
}