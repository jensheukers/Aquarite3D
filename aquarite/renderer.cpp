/**
*	Filename: renderer.h
*
*	Description: Header file for renderer class
*
*	Version: 16/1/2019
*
*	© 2018, Jens Heukers
*/
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "resourcemanager.h"
#include "renderer.h"
#include "core.h"
#include "debug.h"
#include "camera.h"
#include "texture.h"
#include "graphics/light.h"
#include "ui/uielement.h"
#include "ui/text.h"
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"

//Include glText external header file
#define GLT_IMPLEMENTATION
#include "../external/gltext.h"

#define MAX_LIGHTS 25

void GenerateScreenQuadBuffers(unsigned int &vao, unsigned int &vbo) {
	float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	gltViewport(width, height);
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

void Renderer::DrawModel(Camera* camera, Model* model, Vec3 position, Vec3 rotation, Vec3 scale) {
	static Model* lastModel = nullptr; // Note static!, we can only declare once so we can use this property over the course of the program
	bool sameModelAsLastDraw = false;

	if (lastModel == model && model->GetMeshesCount() == 1) sameModelAsLastDraw = true; // Last model was same model, we dont have to bind VAO & Shader program again!

																						//Check if there are as equal meshes as there are materials
	if (model->GetMeshesCount() != model->GetMaterialCount()) {
		if (model->GetMeshesCount() > model->GetMaterialCount())
			Debug::Log("Error: Model " + model->GetName() + " has more meshes than materials", typeid(*this).name());
		else
			Debug::Log("Error: Model " + model->GetName() + " has more materials than meshes", typeid(*this).name());
		return; // Returns if not equal
	}

	for (int i = 0; i < model->GetMeshesCount(); i++) { // For every mesh on the model
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

void Renderer::DrawSprite(Texture* texture, Vec3 position, Vec3 scale) {
	if (texture == nullptr) return;

	float tx = (((float)texture->textureData->width / Core::GetResolution().x) * 2) - 1.f;
	float ty = (((float)texture->textureData->height / Core::GetResolution().y) * 2) - 1.f;

	float quadVertices[] = {
		-1.0f,  ty,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		tx, -1.0f,  1.0f, 0.0f,

		-1.0f,  ty,  0.0f, 1.0f,
		tx, -1.0f,  1.0f, 0.0f,
		tx,  ty,  1.0f, 1.0f
	};

	//Check if vao and vbo are not set, if not we generate buffers
	if (spriteVAO == 0 && spriteVBO == 0) {
		glGenVertexArrays(1, &spriteVAO);
		glGenBuffers(1, &spriteVBO);
		glBindVertexArray(spriteVAO);
		glBindBuffer(GL_ARRAY_BUFFER, spriteVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, spriteVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), &quadVertices);
	}

	glDisable(GL_DEPTH_TEST); // Disable depth testing

	glUseProgram(spriteShader->GetShaderProgram()); // Bind framebuffer shader program
	glBindVertexArray(spriteVAO); // Bind Vertex Array Object

	//Normalize positions to get OpenGL coordinates
	float x = (position.x / Core::GetResolution().x) * 2;
	float y = (position.y / Core::GetResolution().y) * 2;

	//Set uniforms
	spriteShader->SetVec2("position", glm::vec2(x, y));
	spriteShader->SetVec2("scale", glm::vec2(scale.x, scale.y));

	glBindTexture(GL_TEXTURE_2D, texture->GetGLTexture()); // Bind framebuffer texture
	glDrawArrays(GL_TRIANGLES, 0, 6); // Draw quad
}

void Renderer::DrawText(std::string text, Point4f color, Point2f position, float scale) {
	GLTtext* glText = gltCreateText();
	gltSetText(glText, text.c_str());

	gltBeginDraw();

	gltColor(color.x, color.y, color.z, color.w);

	//We want to calculate y position from the Core::GetResolution().x value
	gltDrawText2D(glText, position.x, Core::GetResolution().y - position.y, scale);

	gltEndDraw();
	gltDeleteText(glText);
}

void Renderer::DrawSkybox() {
	//Draw Skybox
	glDepthMask(GL_FALSE); // Disable depth mask
	glDepthFunc(GL_LEQUAL);
	glUseProgram(skybox->GetShader()->GetShaderProgram());

	GLint viewLoc = glGetUniformLocation(skybox->GetShader()->GetShaderProgram(), "view");
	GLint projLoc = glGetUniformLocation(skybox->GetShader()->GetShaderProgram(), "projection");

	glm::mat4 convertedView = glm::mat4(glm::mat3(view)); // remove translation from view matrix
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(convertedView));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(skybox->GetVAO());
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->GetCubeMapTexture());
	glDrawArrays(GL_TRIANGLES, 0, 36); // Draw skybox

	glDepthMask(GL_TRUE); // Enable depth mask
	glDepthFunc(GL_LESS);
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

	//Enable blend / set blend func
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Set clear color to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Create framebuffer instance and set shader
	frameBuffer = new FrameBuffer(Core::GetResolution(), GL_COLOR_ATTACHMENT0);
	frameBuffer->SetShader(ResourceManager::GetShader("_aquariteDefaultFrameBufferShader"));
	frameBuffer->GetShader()->SetInt("screenTexture", 0);

	//Create skybox instance and set shader
	//We wont set any textures, as this is up to the user
	//The skybox is pure a holder
	skybox = new SkyBox();
	skybox->GetShader()->SetInt("skybox", 0);

	//Init sprite shader
	spriteShader = new Shader("shaders/sprite_default.vs", "shaders/sprite_default.fs");

	//Generate screen quad vbo
	GenerateScreenQuadBuffers(screenVAO, screenVBO);

	//Initialize GlText
	gltInit();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//Start a new IMGUI Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Set booleans
	renderFrameBuffer = true; // Draw frame buffer to screen vao as texture by default

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

	//Also clear the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->GetFBO());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color/depth buffer

	glBindVertexArray(0); // Unbind
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind current texture unit

	//Clear drawlist
	for (size_t i = 0; i < drawList.size(); i++) {
		drawList.erase(drawList.begin() + i); // Erase pointer
	}

	for (size_t i = 0; i < uiElementList.size(); i++) {
		uiElementList.erase(uiElementList.begin() + i); // Erase pointer
	}

	for (size_t i = 0; i < textList.size(); i++) {
		textList.erase(textList.begin() + i); // Erase pointer
	}

	//We start a new imgui frame here so we can draw in between frames
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
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

void Renderer::RegisterUIElement(UIElement* element) {
	uiElementList.push_back(element); // Add pointer
}

void Renderer::RegisterText(Text* text) {
	textList.push_back(text);
}

void Renderer::Render(Camera* camera) {
	std::vector<Entity*> renderEntities; // vector will be filled with entities that are ready for draw
	size_t i;
	for (i = 0; i < drawList.size(); i++) { // Do checks
		//We do a frustum culling check and filter out all objects that are not in sight.
		if (!drawList[i]->GetModel()->IgnoreFrustumState()) {
			if (!InFrustum(camera, drawList[i]->GetModel(), drawList[i]->GetPositionGlobal())) continue;
		}
		renderEntities.push_back(drawList[i]);
	}

	// We want to draw from back to front, so we have to do some sorting
	//We can use a map for this and then draw those values to the screen
	std::map<float, Entity*> sorted;
	for (i = 0; i < renderEntities.size(); i++) { //Sort
		float distance = Vec3::Distance(Vec3::ToVec3(camera->GetPos()), renderEntities[i]->position); // Get distance
		sorted[distance] = renderEntities[i]; // Add to map
	}

	//Bind framebuffer and enable depth test
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->GetFBO());
	glEnable(GL_DEPTH_TEST);

	//Render default models
	for (std::map<float, Entity*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) { //Finally draw to screen
		if (it->second->GetModel()->GetDrawMode() != DrawMode::Default) continue; // continue iteration if drawmode is not Default
		DrawModel(camera, it->second->GetModel(), it->second->GetPositionGlobal(), it->second->GetRotationGlobal(), it->second->GetScale());
	}

	DrawSkybox(); // We want to draw the skybox before the late draw calls, this is due to transparancy

	//Render models with late drawmode
	for (std::map<float, Entity*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) { //Finally draw to screen
		if (it->second->GetModel()->GetDrawMode() != DrawMode::Late) continue; // continue iteration if drawmode is not Late
		DrawModel(camera, it->second->GetModel(), it->second->GetPositionGlobal(), it->second->GetRotationGlobal(), it->second->GetScale());
	}

	//Draw all sprites
	for (i = 0; i < uiElementList.size(); i++) {
		DrawSprite(uiElementList[i]->GetImage(), uiElementList[i]->GetPositionGlobal(), uiElementList[i]->GetScale());
	}

	//Disable depth testing (For drawing quad to screen & drawing text)
	glDisable(GL_DEPTH_TEST); // Disable depth testing

	//Draw all texts
	for (i = 0; i < textList.size(); i++) {
		DrawText(textList[i]->GetText(), textList[i]->GetColor(), Point2f(textList[i]->position.x, textList[i]->position.y), textList[i]->GetTextScale());
	}

	//Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer, so we can draw the framebuffer

	//If we can render our framebuffer to the screen vao
	if (renderFrameBuffer) {
		glUseProgram(frameBuffer->GetShader()->GetShaderProgram()); // Bind framebuffer shader program
		glBindVertexArray(screenVAO); // Bind Vertex Array Object

		glBindTexture(GL_TEXTURE_2D, frameBuffer->GetTextureColorBufferObject()); // Bind framebuffer texture
		glDrawArrays(GL_TRIANGLES, 0, 6); // Draw quad
	}

	//ImGui render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLFWwindow* Renderer::GetWindow() {
	return this->window;
}

FrameBuffer* Renderer::GetFrameBuffer() {
	return this->frameBuffer;
}

SkyBox* Renderer::GetSkybox() {
	return this->skybox;
}

glm::mat4 Renderer::GetViewMatrix() {
	return this->view;
}

glm::mat4 Renderer::GetProjectionMatrix() {
	return this->projection;
}

void Renderer::DrawFrameBufferToScreenObject(bool state) {
	this->renderFrameBuffer = state;
}

Renderer::~Renderer() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(this->window);
	glfwTerminate(); // Terminate GLFW
	Debug::Log("GLFW Terminated", typeid(*this).name()); // Log
}