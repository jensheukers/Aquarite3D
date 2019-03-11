/**
*	Filename: core.h
*
*	Description: Header file for Core class.
*
*	Version: 28/2/2019
*
*	© 2019, Jens Heukers
*/

#ifndef CORE_H
#define CORE_H
#include <iostream>
#include <thread>
#include <string>
#include "math/pointx.h"
#include "renderer.h"

/**
* Thread struct, holds a extra boolean, so we can check if execution is done
*/
struct Thread {
	std::thread thread; /**< Pointer to the thread*/
	bool isDone; /**< Boolean if true thread is done executing*/
};

class Core {
private:
	//Static Variables
	static Core* _instance; /// @brief Static core singleton instance.
	static std::string _executablePath; /// @brief The absolute path to the executable build folder

	//instance Active variable
	bool _active; /// @brief Defines if core is still activly running.

	//Resolution
	Point2i _resolution; /// @brief The resolution of the canvas
	float _fov; /// @brief The field of view

	// Time variables
	unsigned _timeStart; /// @brief The time in milliseconds when the Initialize() method was called
	unsigned _timeElapsed; /// @brief The time elapsed since Initialize() method was called. Time is in milliseconds
	float _deltaTime; /// @brief The time it took for a frame to render.
	
	//Variables for FPS calculation
	unsigned _fps; /// @brief The amount of frames rendered to screen in a second.
	unsigned _frames; /// @brief Stores the amount of frames rendered in a certain time, will be reset to 0 every 1000 milliseconds
	unsigned _lastFrameUpdate; /// @brief The time when the framerate was updated.

	//Other components
	Renderer* renderer; /// @brief Renderer Instance

	//Cursor state
	bool cursorEnabled; /// @brief true if cursor is enabled

	//List of threads, we check each frame if a thread has done its execution, so we can end it
	std::vector<Thread*> threads; /**< List of activly running threads*/

	//We want to keep a list of entities, so we can easily access all entities, whenever a child is added to whatever entity,
	//A call should be made to this list (Only applies for Entities base, not UIElements)
	std::vector<Entity*> entityList; /**< List of entityes*/


public:
	//Static methods

	/**
	* Returns the singleton instance, if non is existant it will create a new instance.
	*/
	static Core* GetInstance();

	/**
	* Returns the absolute executable path
	*/
	static std::string GetBuildDirectory();

	/**
	* Returns the amount of time between a frame
	*/
	static float GetDeltaTime();

	/**
	* Returns the amount of frames per second
	*/
	static float GetFPS();

	//Non-Static methods

	/**
	* Initializes the Core, initializes the renderer and more. Takes in program arguments array as parameter.
	*/
	int Initialize(char* argv[], Point2i resolution = Point2i(800,600));

	/**
	* Handles all the updates and creates game loop.
	*/
	void HandleUpdates();

	/**
	* Caclulates the DeltaTime then returns
	*/
	float CalculateDeltaTime();

	/**
	* Destroys the Core instance
	*/
	static void Destroy();

	/**
	* Returns true if Core is still active, else returns false.
	*/
	bool Active();

	/**
	* Parses Light instance to renderer
	*/
	void HandleLightRegister(Light* light, int state);

	/**
	* Enable/Disable cursor
	*/
	static void SetCursorEnabled(bool state);

	/**
	* Returns true if cursor is enabled else returns false
	*/
	static bool CursorEnabled();

	/**
	* Returns the resolution as a Point2i
	*/
	static Point2i GetResolution();

	/**
	* Gets the current active GLFW window from the renderer then returns
	*/
	static GLFWwindow* GetActiveWindow();

	/**
	* Sets the resolution refernence, method is mostly called by the renderer
	*/
	static void SetResolutionReference(Point2i res);

	/**
	* Sets the field of view
	*/
	static void SetFov(float amount);

	/**
	* Returns the field of view
	*/
	static float GetFov();

	/**
	* Returns the time elapsed since start of program
	*/
	static unsigned GetTimeElapsed();

	/**
	* Returns the skybox of the renderer
	*/
	static SkyBox* GetRendererSkybox();

	/**
	* Returns the framebuffer of the renderer
	*/
	static FrameBuffer* GetRendererFrameBuffer();

	/**
	* Returns the renderer's view matrix
	*/
	static glm::mat4 GetRendererViewMatrix();

	/**
	* Returns the renderer's projection matrix
	*/
	static glm::mat4 GetRendererProjectionMatrix();

	/**
	* Defines if the renderer should draw the framebuffer to its screen vertex array object as a texture
	*/
	static void SetRendererDrawFrameBufferToScreen(bool state);

	/**
	* Registers a new thread to keep hold of
	*/
	static void RegisterThread(Thread* thread);

	/**
	* Destroys the thread where index matches, returns string with info
	*/
	static std::string DestroyThread(int index);

	/**
	* Adds a entity to the entity global list, Note that it is adviced to use AddChild, to allow for rendering
	*/
	static void AddToGlobalEntityList(Entity* entity);

	/**
	* Returns the global entity vector
	* @return std::vector<Entity*>
	*/
	static std::vector<Entity*> GetGlobalEntityList();

	/**
	* Removes a entity from the global entity vector
	*/
	static void RemoveFromGlobalEntityList(Entity* entity);
};

#endif // !CORE_H
