/**
*	Filename: renderer.cpp
*
*	Description: Source file for renderer class
*
*	Version: 13/1/2019
*
*	© 2018, Jens Heukers
*/
#include <string>
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "renderer.h"
#include "core.h"
#include "debug.h"
#include "glm/glm.hpp"
#include "model.h"
#include "graphics/light.h"

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Core::SetResolutionReference(Point2i(width, height));
}

int Renderer::Initialize(std::string windowTitle, int width, int height) {

	this->screenWidth = width;
	this->screenHeight = height;

	//Initialize GLFW
	if (!glfwInit()) {
		Debug::Log("GLFW failed to initialize", typeid(*this).name());
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create Window Context
	_window = glfwCreateWindow(width,height,windowTitle.c_str(), NULL, NULL);
	glfwMakeContextCurrent(_window); // Make the current context

	//Initialize GLEW
	if (glewInit() != GLEW_OK) {
		Debug::Log("Glew failed to initialize", typeid(*this).name());
		return 1;
	}
	
	glEnable(GL_DEPTH_TEST);

	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	//Setup viewport
	glViewport(0,0,width,height);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glfwSetFramebufferSizeCallback(_window, FrameBufferSizeCallback);
	return 0;
}

void Renderer::Update(Camera* camera) {
	//Translate Camera Vectors to GLM vectors so OpenGL understands
	glm::vec3 cameraPos = glm::vec3(camera->GetPos().x, camera->GetPos().y, camera->GetPos().z); // Position
	glm::vec3 cameraTarget = glm::vec3(camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z); // Front
	glm::vec3 cameraUp = glm::vec3(camera->GetUp().x, camera->GetUp().y, camera->GetUp().z); // Up

	//View matrix
	view = glm::lookAt(cameraPos, cameraPos + cameraTarget, cameraUp);

	//Projection matrix
	projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	camera->GetFrustum()->setCamInternals(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
}

void Renderer::DrawSprite(Model* model, Vec3 position, Vec3 rotation, Vec3 scale) {
	for (int i = 0; i < model->GetMeshesCount(); i++) {
		if (model->GetMaterial(i) == nullptr) return;

		glUseProgram(model->GetMaterial(i)->GetShader()->GetShaderProgram());

		if (model->GetMesh(i)->GetVAO() == NULL) return;

		glBindVertexArray(model->GetMesh(i)->GetVAO());

		if (model->GetMaterial(i)->GetDiffuse()) {
			glBindTexture(GL_TEXTURE_2D, model->GetMaterial(i)->GetDiffuse()->GetGLTexture());
		}

		//Handle transformations
		// Note that z axis is non existant, this is due to sprites are always to be rendered in 2D
		model->GetMaterial(i)->GetShader()->SetVec2("scale", glm::vec2(scale.x, scale.y)); // scale
		model->GetMaterial(i)->GetShader()->SetVec2("position", glm::vec2(position.x, position.y)); // Pos

		glDrawArrays(GL_TRIANGLES, 0, model->GetMesh(i)->GetVerticesCount()); // Draw

		//Cleanup
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Renderer::DrawModel(Camera* camera, Model* model, Vec3 position, Vec3 rotation, Vec3 scale) {
	//Calculate distance from camera to model
	//Some math here that somehow magicly works, lets just keep calling this magic it does its thing :)
	float sphereSize = model->GetSphereRadius();
	sphereSize += position.Distance(Vec3::ToVec3(camera->GetPos()));
	Vec3 calculatedPos;
	calculatedPos.Set(position);
	calculatedPos = *calculatedPos.Minus(Vec3::ToVec3(camera->GetPos()));

	//If we dont want to ignore frustum state
	if (!model->IgnoreFrustumState()) {
		//If the sphere is in our frustum
		if (camera->GetFrustum()->sphereInFrustum(calculatedPos, sphereSize) == Frustum::OUTSIDE) return;
	}
	//Then continue

	for (int i = 0; i < model->GetMeshesCount(); i++) {
		// Prepare Draw
		if (model->GetMaterialCount() < 1) return;

		if (!model->GetMaterial(i)) return;

		glm::mat4 modelTransform(1);
		modelTransform = glm::translate(modelTransform, glm::vec3(position.x, position.y, position.z)); // position
		modelTransform = glm::rotate(modelTransform, Vec3::DegToRad(rotation.x), glm::vec3(1, 0, 0)); // Rotation
		modelTransform = glm::rotate(modelTransform, Vec3::DegToRad(rotation.y), glm::vec3(0, 1, 0)); // Rotation
		modelTransform = glm::rotate(modelTransform, Vec3::DegToRad(rotation.z), glm::vec3(0, 0, 1)); // Rotation
		modelTransform = glm::scale(modelTransform, glm::vec3(scale.x, scale.y, scale.z)); // scale
		
		// Define shader program 
		glUseProgram(model->GetMaterial(i)->GetShader()->GetShaderProgram());

		//Handle lighting
		glm::vec3 diffuseColor = model->GetMaterial(i)->GetColor() * model->GetMaterial(i)->GetDiffuseColor();
		glm::vec3 ambientColor = diffuseColor * model->GetMaterial(i)->GetAmbientColor();

		//Turn off directional Lighting
		model->GetMaterial(i)->GetShader()->SetVec3("dirLight.direction", glm::vec3(0, 0, 0));
		model->GetMaterial(i)->GetShader()->SetVec3("dirLight.ambient", glm::vec3(0, 0, 0));
		model->GetMaterial(i)->GetShader()->SetVec3("dirLight.diffuse", glm::vec3(0, 0, 0));
		model->GetMaterial(i)->GetShader()->SetVec3("dirLight.specular", glm::vec3(0, 0, 0));

		model->GetMaterial(i)->GetShader()->SetVec3("material.ambientColor", ambientColor);
		model->GetMaterial(i)->GetShader()->SetVec3("material.diffuseColor", diffuseColor);
		model->GetMaterial(i)->GetShader()->SetVec3("material.specular", model->GetMaterial(i)->GetSpecular());
		model->GetMaterial(i)->GetShader()->SetFloat("material.shininess", model->GetMaterial(i)->GetShine());

		for (size_t n = 0; n < MAX_LIGHTS; n += 1) {
			if (_lights.size() > MAX_LIGHTS) continue; // make sure we dont render more than MAX LIGHTS

			if (n >= _lights.size()) {
				std::string _positionString = "pointLights[";
				_positionString.append(std::to_string(n));
				_positionString.append("]");
				model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".position", glm::vec3(0,0,0));
				model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".ambient", glm::vec3(0, 0, 0));
				model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".diffuse", glm::vec3(0, 0, 0));
				model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".specular", glm::vec3(0, 0, 0));
				continue;
			}

			if (_lights[n]->GetLightType() == LightType::PointLight) {
				std::string _positionString = "pointLights[";
				_positionString.append(std::to_string(n));
				_positionString.append("]");

				//Handle 
				model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".position", glm::vec3(_lights[n]->position.x, _lights[n]->position.y, _lights[n]->position.z));
				model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".ambient", _lights[n]->GetAmbient());
				model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".diffuse", _lights[n]->GetDiffuse());
				model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".specular", _lights[n]->GetSpecular());
			}

			if (_lights[n]->GetLightType() == LightType::Directional) {
				// Handle Directional Light, also cast down
				DirectionalLight* light = static_cast<DirectionalLight*>(_lights[n]);
				model->GetMaterial(i)->GetShader()->SetVec3("dirLight.direction", light->GetDirection());
				model->GetMaterial(i)->GetShader()->SetVec3("dirLight.ambient", light->GetAmbient());
				model->GetMaterial(i)->GetShader()->SetVec3("dirLight.diffuse", light->GetDiffuse());
				model->GetMaterial(i)->GetShader()->SetVec3("dirLight.specular", light->GetSpecular());
			}
		}

		model->GetMaterial(i)->GetShader()->SetVec3("viewPos", camera->GetPos());

		//Bind VAO
		if (model->GetMesh(i)->GetVAO() == NULL) return;

		glBindVertexArray(model->GetMesh(i)->GetVAO());

		if (model->GetMaterial(i)->GetDiffuse()) {
			model->GetMaterial(i)->GetShader()->SetBool("hasTexture", true); // Set hasTexture to true
			glBindTexture(GL_TEXTURE_2D, model->GetMaterial(i)->GetDiffuse()->GetGLTexture());
		}
		else {
			model->GetMaterial(i)->GetShader()->SetBool("hasTexture", false); // Set hasTexture to true
		}

		//Handle transformations in vertex shader
		GLint modelLoc = glGetUniformLocation(model->GetMaterial(i)->GetShader()->GetShaderProgram(), "model");
		GLint viewLoc = glGetUniformLocation(model->GetMaterial(i)->GetShader()->GetShaderProgram(), "view");
		GLint projLoc = glGetUniformLocation(model->GetMaterial(i)->GetShader()->GetShaderProgram(), "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTransform));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw
		glDrawArrays(GL_TRIANGLES, 0, model->GetMesh(i)->GetVerticesCount());

		//Cleanup
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Renderer::AddLight(Light* light) {
	if (light->GetLightType() == LightType::NONE) return; // If lighttype == NONE then return
	_lights.push_back(light);
}

void Renderer::RemoveLight(Light* light) {
	for (size_t i = 0; i < _lights.size(); i++) {
		if (light == _lights[i]) {
			_lights.erase(_lights.begin() + i);
		}
	}
}

void Renderer::SwapBuffers() {
	glfwSwapBuffers(_window);
}

void Renderer::PollEvents() {
	glfwPollEvents();
}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // Reset the matrix
}

GLFWwindow* Renderer::GetWindow() {
	return this->_window;
}

void Renderer::EnableCursor(bool state) {
	if (state) {
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}

	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Renderer::~Renderer() {
	glfwTerminate(); // Terminate GLFW
	Debug::Log("GLFW Terminated", typeid(*this).name()); // Log
}