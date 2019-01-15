/**
*	Filename: renderer.cpp
*
*	Description: Source file for renderer class
*
*	Version: 13/1/2019
*
*	© 2018, Jens Heukers
*/
#ifndef RENDERER_H
#define RENDERER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "math/vec3.h"
#include "math/pointx.h"
#include "graphics/framebuffer.h"

//Forward declarations
class Entity;
class Camera;
class Model;
class Light;
class Texture;

class Renderer {
private:
	GLFWwindow * window; /// @brief The window of the renderer
	std::vector<Entity*> drawList; /// @brief The vector of entities to be drawn, will reset each frame
	std::vector<Light*> lights; /// @brief Vector containing lights.
	glm::mat4 view, projection; /// @brief The view and projection matrixes
	FrameBuffer* frameBuffer; /// @brief The framebuffer instance of the renderer

	//We need to create a screen vbo so we can render our scene to a quad, for post processing purposes
	unsigned int screenVAO, screenVBO; /// @brief Screen Vertex Array Object, Screen Vertex Buffer Object

	/**
	* Returns true if model is indeed inside our frustum
	*/
	bool InFrustum(Camera* camera, Model* model, Vec3 position);

	/**
	* Handles the lighting in the shader, i defines the currently rendered material/mesh
	*/
	void HandleShaderLighting(Model* model, int i);

	/**
	* Renders a model to the screen
	*/
	void DrawModel(Camera* camera, Model* model, Vec3 position, Vec3 rotation, Vec3 scale);
public:
	/**
	* Sets up window context, sets up OpenGL properties
	*/
	int Initialize(const char* windowTitle, int width, int height);

	/**
	* Handles some matrix translations such as camera fov ect, method is called from core
	*/
	void HandleTranslations(Camera* camera, float fov);

	/**
	* Enables/Disables the cursor
	*/
	void EnableCursor(bool state);

	/**
	* Swaps the buffers
	*/
	void SwapBuffers();

	/**
	* Poll glfw events
	*/
	void PollEvents();

	/**
	* Clears the screen to be ready for next frame, also clears the drawList
	*/
	void Clear();

	/**
	* Adds a light to the lights vector
	*/
	void AddLight(Light* light);

	/**
	* Removes a light from the lights vector
	*/
	void RemoveLight(Light* light);


	/**
	* Registers a entity to the drawList
	*/
	void RegisterEntity(Entity* entity);

	/**
	* Prepares and renders the entire drawList
	*/
	void Render(Camera* camera);

	/**
	* Draws a 2d sprite on the screen
	*/
	void DrawSprite(Model* model, Vec3 position, Vec3 rotation, Vec3 scale);
	
	/**
	* Returns the GLFW Window
	*/
	GLFWwindow* GetWindow();

	/**
	* Returns the framebuffer
	*/
	FrameBuffer* GetFrameBuffer();

	/**
	* Destructor
	*/
	~Renderer();
};

#endif // !RENDERER_H