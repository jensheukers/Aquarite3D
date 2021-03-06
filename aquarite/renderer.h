/**
*	Filename: renderer.cpp
*
*	Description: Source file for renderer class
*
*	Version: 16/1/2019
*
*	� 2018, Jens Heukers
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
#include "graphics/cubemap.h"

//Forward declarations
class Entity;
class UIElement;
class Text;
class Camera;
class Model;
class Light;
class Texture;

class Renderer {
private:
	GLFWwindow * window; /// @brief The window of the renderer
	std::vector<Entity*> drawList; /// @brief The vector of entities to be drawn, will reset each frame
	std::vector<UIElement*> uiElementList; /// @brief The vector of sprites to be drawn, will reset each frame
	std::vector<Text*> textList; /// @brief The vector of text to be drawn, will reset each frame
	std::vector<Light*> lights; /// @brief Vector containing lights.
	glm::mat4 view, projection; /// @brief The view and projection matrixes

	//Custom object holders
	FrameBuffer* frameBuffer; /// @brief The framebuffer instance of the renderer
	SkyBox* skybox; /// @brief The skybox to be rendered.

	//Shader used for drawing sprites
	Shader* spriteShader; ///@brief The shader used to draw sprites

	//We need to create a screen vbo so we can render our scene to a quad, for post processing purposes
	unsigned int screenVAO, screenVBO; /// @brief Screen Vertex Array Object, Screen Vertex Buffer Object
	unsigned int spriteVAO, spriteVBO; /// @brief Sprite VBO and VAO, will be rebuffered each sprite draw, to fit size

	//Booleans
	bool renderFrameBuffer; /// @brief If true, the frameBuffer will be rendered to screen quad, and displayed

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

	/**
	* Draws a 2d sprite on the screen
	*/
	void DrawSprite(Texture* texture, Vec3 position, Vec3 scale);

	/**
	* Draws text to the screen
	*/
	void DrawText(std::string text, Point4f colour, Point2f position, float scale);

	/**
	* Renders the skybox
	*/
	void DrawSkybox();
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
	* Registers a sprite to the spriteList
	*/
	void RegisterUIElement(UIElement* sprite);

	/**
	* Registers a text to the textList
	*/
	void RegisterText(Text* sprite);

	/**
	* Prepares and renders the entire drawList
	*/
	void Render(Camera* camera);

	/**
	* Returns the GLFW Window
	*/
	GLFWwindow* GetWindow();

	/**
	* Returns the framebuffer
	*/
	FrameBuffer* GetFrameBuffer();

	/**
	* Returns the skybox
	*/
	SkyBox* GetSkybox();

	/**
	* Returns the current view matrix
	*/
	glm::mat4 GetViewMatrix();

	/**
	* Returns the current projection matrix
	*/
	glm::mat4 GetProjectionMatrix();

	/**
	* Defines if framebuffer should be rendered to screen quad or not, this method is usefull for creating seperate windows 
	* i.e if user wants the framebuffer drawn to a texture.
	*/
	void DrawFrameBufferToScreenObject(bool state);

	/**
	* Destructor
	*/
	~Renderer();
};

#endif // !RENDERER_H