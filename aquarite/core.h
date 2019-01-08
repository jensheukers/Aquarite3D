/**
*	Filename: core.h
*
*	Description: Header file for Core class.
*
*	Version: 30/12/2018
*
*	© 2018, Jens Heukers
*/

#ifndef CORE_H
#define CORE_H
#include <iostream>
#include <string>
#include "math/pointx.h"
#include "renderer.h"

class Core {
private:
	//Static Variables
	static Core* _instance; /// @brief Static core singleton instance.
	static std::string _executablePath; /// @brief The absolute path to the executable build folder

	//instance Active variable
	bool _active; /// @brief Defines if core is still activly running.

	//Resolution
	Point2i _resolution; /// @brief The resolution of the canvas

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
	* Returns the resolution as a Point2i
	*/
	static Point2i GetResolution();

	/**
	* Sets the resolution refernence, method is mostly called by the renderer
	*/
	static void SetResolutionReference(Point2i res);

	/**
	* Returns the time elapsed since start of program
	*/
	static unsigned GetTimeElapsed();
};

#endif // !CORE_H
