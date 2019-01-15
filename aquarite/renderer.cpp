/**
*	Filename: renderer.h
*
*	Description: Header file for renderer class
*
*	Version: 15/1/2019
*
*	� 2018, Jens Heukers
*/
#include <glm/gtc/type_ptr.hpp>
#include "renderer.h"
#include "core.h"
#include "debug.h"
#include "camera.h"
#include "texture.h"
#include "graphics/light.h"

#define MAX_LIGHTS 25

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Core::SetResolutionReference(Point2i(width, height));
}

bool Renderer::InFrustum(Camera* camera, Model* model, Vec3 position) {
	//Calculate distance from camera to model
	//Some math here that somehow magicly works, lets just keep calling this magic it does its thing :)
	float sphereSize = model->GetSphereRadius();

	//If we dont want to ignore frustum state
	if (!model->IgnoreFrustumState()) {
		//If the sphere is in our frustum
		if (camera->GetFrustum()->sphereInFrustum(position, sphereSize) == Frustum::OUTSIDE) return false;
	}

	return true; // Pass test
}

void Renderer::HandleShaderLighting(Model* model, int i) {
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
		if (lights.size() > MAX_LIGHTS) continue; // make sure we dont render more than MAX LIGHTS

		if (n >= lights.size()) {
			std::string _positionString = "pointLights[";
			_positionString.append(std::to_string(n));
			_positionString.append("]");
			model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".position", glm::vec3(0, 0, 0));
			model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".ambient", glm::vec3(0, 0, 0));
			model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".diffuse", glm::vec3(0, 0, 0));
			model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".specular", glm::vec3(0, 0, 0));
			continue;
		}

		if (lights[n]->GetLightType() == LightType::PointLight) {
			std::string _positionString = "pointLights[";
			_positionString.append(std::to_string(n));
			_positionString.append("]");

			//Handle 
			model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".position", glm::vec3(lights[n]->position.x, lights[n]->position.y, lights[n]->position.z));
			model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".ambient", lights[n]->GetAmbient());
			model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".diffuse", lights[n]->GetDiffuse());
			model->GetMaterial(i)->GetShader()->SetVec3(_positionString + ".specular", lights[n]->GetSpecular());
		}

		if (lights[n]->GetLightType() == LightType::Directional) {
			// Handle Directional Light, also cast down
			DirectionalLight* light = static_cast<DirectionalLight*>(lights[n]);
			model->GetMaterial(i)->GetShader()->SetVec3("dirLight.direction", light->GetDirection());
			model->GetMaterial(i)->GetShader()->SetVec3("dirLight.ambient", light->GetAmbient());
			model->GetMaterial(i)->GetShader()->SetVec3("dirLight.diffuse", light->GetDiffuse());
			model->GetMaterial(i)->GetShader()->SetVec3("dirLight.specular", light->GetSpecular());
		}
	}
}

int Renderer::Initialize(const char* windowTitle, int width, int height) {
	//Initialize GLFW
	if (!glfwInit()) {
		Debug::Log("GLFW failed to initialize", typeid(*this).name());
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create glfw window
	window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	glfwMakeContextCurrent(window); // Make current context

	//Initialize GLEW
	if (glewInit() != GLEW_OK) {
		Debug::Log("Glew failed to initialize", typeid(*this).name());
		return 1;
	}

	//Setup viewport
	glViewport(0, 0, width, height);
	Core::SetResolutionReference(Point2i(width, height));

	//Set framebuffer callback
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	//Enable depth test / blend / set blend func
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Cull back faces
	glCullFace(GL_BACK);

	//Set clear color to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	return 0; // Return 0 (No errors)
}

void Renderer::HandleTranslations(Camera* camera, float fov) {
	//Get the resolution from core
	Point2i size = Core::GetResolution();

	//Translate Camera Vectors to GLM vectors so OpenGL understands
	glm::vec3 cameraPos = glm::vec3(camera->GetPos().x, camera->GetPos().y, camera->GetPos().z); // Position
	glm::vec3 cameraTarget = glm::vec3(camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z); // Front
	glm::vec3 cameraUp = glm::vec3(camera->GetUp().x, camera->GetUp().y, camera->GetUp().z); // Up

	
	//View matrix
	view = glm::lookAt(cameraPos, cameraPos + cameraTarget, cameraUp);

	//Projection matrix
	projection = glm::perspective(glm::radians(fov), (float)size.x / (float)size.y, 0.1f, 100.0f);
	camera->GetFrustum()->setCamInternals(fov, (float)size.x / (float)size.y, 0.1f, 100.0f);
}

void Renderer::EnableCursor(bool state) {
	if (state) { // Enable cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Disable
}

void Renderer::SwapBuffers() {
	glfwSwapBuffers(window);
}

void Renderer::PollEvents() {
	glfwPollEvents();
}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color/depth buffer
	glLoadIdentity(); // reset matrix

	glBindVertexArray(0); // Unbind
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind current texture unit

	//Clear drawlist
	for (size_t i = 0; i < drawList.size(); i++) {
		drawList.erase(drawList.begin() + i); // Erase pointer
	}
}

void Renderer::AddLight(Light* light) {
	if (light->GetLightType() == LightType::NONE) return; // If lighttype == NONE then return
	lights.push_back(light);
}

void Renderer::RemoveLight(Light* light) {
	for (size_t i = 0; i < lights.size(); i++) {
		if (light == lights[i]) {
			lights.erase(lights.begin() + i);
		}
	}
}

void Renderer::RegisterEntity(Entity* entity) {
	drawList.push_back(entity); // Add pointer
}

void Renderer::RenderDrawList(Camera* camera) {
	std::vector<Entity*> renderEntities; // vector will be filled with entities that are ready for draw
	size_t i;
	for (i = 0; i < drawList.size(); i++) { // Do checks
		//We do a frustum culling check and filter out all objects that are not in sight.
		if (!InFrustum(camera, drawList[i]->GetModel(), drawList[i]->GetPositionGlobal())) continue;
		renderEntities.push_back(drawList[i]);
	}

	for (i = 0; i < renderEntities.size(); i++) { // Draw everything
		DrawModel(camera, renderEntities[i]->GetModel(), renderEntities[i]->GetPositionGlobal(), renderEntities[i]->GetRotationGlobal(), renderEntities[i]->GetScale());
	}
}

void Renderer::DrawModel(Camera* camera, Model* model, Vec3 position, Vec3 rotation, Vec3 scale) {
	static Model* lastModel = nullptr; // Note static!, we can only declare once so we can use this property over the course of the program
	bool sameModelAsLastDraw = false;

	if (lastModel == model && model->GetMeshesCount() == 1) sameModelAsLastDraw = true; // Last model was same model, we dont have to bind VAO & Shader program again!

	//Check if there are as equal meshes as there are materials
	if (model->GetMeshesCount() != model->GetMaterialCount()) {
		if(model->GetMeshesCount() > model->GetMaterialCount())
			Debug::Log("Error: Model " + model->GetName() + " has more meshes than materials", typeid(*this).name());
		else 
			Debug::Log("Error: Model " + model->GetName() + " has more materials than meshes", typeid(*this).name());
		return; // Returns if not equal
	}

	for (size_t i = 0; i < model->GetMeshesCount(); i++) { // For every mesh on the model
		if (!sameModelAsLastDraw) { // If model is different we have to bind some stuff
			glUseProgram(model->GetMaterial(i)->GetShader()->GetShaderProgram()); // Use shader program

			if (model->GetMesh(i)->GetVAO() == NULL) return; // If the Vertex Array Object equals NULL return

			glBindVertexArray(model->GetMesh(i)->GetVAO());
			if (model->GetMaterial(i)->GetDiffuse()) {
				model->GetMaterial(i)->GetShader()->SetBool("hasTexture", true); // Set hasTexture to true
				glBindTexture(GL_TEXTURE_2D, model->GetMaterial(i)->GetDiffuse()->GetGLTexture());
			}
			else {
				model->GetMaterial(i)->GetShader()->SetBool("hasTexture", false); // Set hasTexture to true
			}
		}

		//Handle shader lighting
		HandleShaderLighting(model, i);

		//Model transform
		glm::mat4 modelTransform(1);
		modelTransform = glm::translate(modelTransform, glm::vec3(position.x, position.y, position.z)); // position
		modelTransform = glm::rotate(modelTransform, Vec3::DegToRad(rotation.x), glm::vec3(1, 0, 0)); // Rotation
		modelTransform = glm::rotate(modelTransform, Vec3::DegToRad(rotation.y), glm::vec3(0, 1, 0)); // Rotation
		modelTransform = glm::rotate(modelTransform, Vec3::DegToRad(rotation.z), glm::vec3(0, 0, 1)); // Rotation
		modelTransform = glm::scale(modelTransform, glm::vec3(scale.x, scale.y, scale.z)); // scale

		//Handle view position and matrixes
		model->GetMaterial(i)->GetShader()->SetVec3("viewPos", camera->GetPos());
		GLint modelLoc = glGetUniformLocation(model->GetMaterial(i)->GetShader()->GetShaderProgram(), "model");
		GLint viewLoc = glGetUniformLocation(model->GetMaterial(i)->GetShader()->GetShaderProgram(), "view");
		GLint projLoc = glGetUniformLocation(model->GetMaterial(i)->GetShader()->GetShaderProgram(), "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTransform));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw
		glDrawArrays(GL_TRIANGLES, 0, model->GetMesh(i)->GetVerticesCount());
	}
}

void Renderer::DrawSprite(Model* texture, Vec3 position, Vec3 rotation, Vec3 scale) {

}

GLFWwindow* Renderer::GetWindow() {
	return this->window;
}

Renderer::~Renderer() {
	glfwTerminate(); // Terminate GLFW
	Debug::Log("GLFW Terminated", typeid(*this).name()); // Log
}