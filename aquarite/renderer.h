/**
*	Filename: renderer.h
*
*	Description: Header file for renderer class
*
*	Version: 13/1/2019
*
*	© 2018, Jens Heukers
*/
#ifndef RENDERER_H
#define RENDERER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "camera.h"
#include "mesh.h"
#include "math/vec3.h"

class Model; // Forward declaration

class Light; // Forward declaration

class Renderer {
private:
	GLFWwindow* _window; /// @brief The GLFW window context
	int screenWidth, screenHeight; /// @brief The screen width and Height
	std::vector<Light*> _lights; /// @brief Lights vector
	const size_t MAX_LIGHTS = 25; /// @brief The maximum numbers of lights allowed to be drawn
	glm::mat4 view; /// @Brief The view matrix
	glm::mat4 projection; /// @Brief The projection matrix
public:
	/**
	* Initialize the Renderer, Initialize GLFW & GLEW and create a window context.
	*/
	int Initialize(std::string windowTitle, int width, int height);


	/**
	* Sets up view and projection matrix for this frame
	*/
	void Update(Camera* camera);

	/**
	* Renders a sprite on the screen, uses a different shader than mesh rendering 
	*/
	void DrawSprite(Model* model, Vec3 position, Vec3 rotation, Vec3 scale);

	/**
	* Renders a mesh on the screen, If shader parameter is null, default shader will be used
	*/
	void DrawModel(Camera* camera, Model* model,Vec3 position, Vec3 rotation, Vec3 scale);

	/**
	* Adds a light to the renderer's lights list, also edits the shaderfile, if type is point
	*/
	void AddLight(Light* light);

	/**
	* Removes a light from from the lights list.
	*/
	void RemoveLight(Light* light);

	/**
	* Swap the buffers.
	*/
	void SwapBuffers();

	/**
	* Poll all the GLFW events
	*/
	void PollEvents();

	/**
	* Clear the OpenGL color buffer
	*/
	void Clear();

	/**
	* Returns the GLFW Window
	*/
	GLFWwindow* GetWindow();

	/**
	* Sets the cursor mode
	*/
	void EnableCursor(bool state);

	/**
	* Destructor
	*/
	~Renderer();
};

#endif // !RENDERER_H
