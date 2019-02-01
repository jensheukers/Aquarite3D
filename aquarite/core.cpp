/**
*	Filename: core.cpp
*
*	Description: Source file for Core class.
*
*	Version: 23/1/2019
*
*	© 2018, Jens Heukers
*/
#include <Windows.h>
#include <chrono>
#include "soundmanager.h"
#include "core.h"
#include "scenemanager.h"
#include "resourcemanager.h"
#include "input.h"
#include "debug.h"

Core* Core::_instance; // Declare static member
std::string Core::_executablePath; // Declare static member

Core* Core::GetInstance() {
	if (_instance) { // If instance exists
		return _instance; // Return instance
	}

	_instance = new Core(); // Else create a new instance
	return _instance; // Return instance
}

std::string Core::GetBuildDirectory() {
	return _executablePath; // Return the executable path
}

float Core::GetDeltaTime() {
	return Core::GetInstance()->_deltaTime; // Return _deltaTime
}

float Core::GetFPS() {
	return (float)Core::GetInstance()->_fps; // return fps
}

int Core::Initialize(char* argv[], Point2i resolution) {
	Debug::Log("Initializing", typeid(*this).name());

	//Find the executable folder location
	std::string _exeDirArg = argv[0]; // Get the executable path directory from arguments
	std::size_t found = _exeDirArg.find_last_of("/\\"); // Get position of character
	_executablePath = _exeDirArg.substr(0, found); // Cut off last part of path
	_executablePath.append("\\"); // Append a slash to return the absolute directory

	//Set up time
	this->_timeStart = (unsigned)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(); // Recieve Current Time
	this->_timeElapsed = this->_timeStart - this->_timeStart; // Calculate Time elapsed
	
	//Setup DeltaTime
	this->_deltaTime = 0;

	//Initialize frame calculation variables
	this->_frames = 0;
	this->_lastFrameUpdate = this->_timeElapsed;
	this->_fps = 0;

	//Resolution
	this->_resolution = resolution;
	this->_fov = 45.0f; // Set fov to 45.0f by default

	// Initialize Renderer
	renderer = new Renderer();
	if (renderer->Initialize("Aquarite", _resolution.x, _resolution.y) != 0) {
		Debug::Log("Renderer has failed to Initialize", typeid(Core).name());
		return 1;
	}

	//Set cursorEnabled to true by default
	this->cursorEnabled = true;

	// Initialize SceneManager, (if end user has not done it yet)
	SceneManager::GetInstance();

	// Initialize ResourceManager, (if end user has not done it yet)
	ResourceManager::GetInstance();

	// Initialize Input, (if end user has not done it yet)
	Input::Init(renderer->GetWindow());

	// Initialize SoundManager, (if end user has not done it yet)
	SoundManager::Init();

	this->_active = true; // set active to true
	Debug::Log("Initialized", typeid(*this).name());
	return 0;
}

void Core::HandleUpdates() {
	//Calculate DeltaTime
	this->_deltaTime = this->CalculateDeltaTime();

	//Calculate Time
	unsigned _currentTime = (unsigned)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(); // Recieve Current Time
	this->_timeElapsed = _currentTime - this->_timeStart; // Calculate Time elapsed

	if (this->_timeElapsed >= this->_lastFrameUpdate + 1000) { // If _lastFrameUpdate is more or equal to 1 second ago
		this->_fps = _frames; // Set _fps to frames
		this->_frames = 0; // Set _frames back to 0
		this->_lastFrameUpdate = this->_timeElapsed; // Set last frame update to the time elapsed
	}

	float _localDelta = (float)this->_timeElapsed; // Set local delta to current time elapsed

	//Update Input
	Input::HandleUpdates();

	//Update Game
	if (SceneManager::GetActiveScene()) {
		// Update all entities and their children
		if (SceneManager::GetActiveScene()->GetActiveCamera()) {
			//Update SoundManager
			SoundManager::Update(Vec3::ToVec3(SceneManager::GetActiveScene()->GetActiveCamera()->GetPos()), 
								 Vec3::ToVec3(SceneManager::GetActiveScene()->GetActiveCamera()->GetTarget()), 
								 Vec3::ToVec3(SceneManager::GetActiveScene()->GetActiveCamera()->GetUp()));

			SceneManager::GetActiveScene()->GetActiveCamera()->UpdateFront();
			//Update renderer
			renderer->HandleTranslations(SceneManager::GetActiveScene()->GetActiveCamera(), _fov);

			//Update frustum
			Vec3 cameraPos = Vec3::ToVec3(SceneManager::GetActiveScene()->GetActiveCamera()->GetPos());
			Vec3 cameraTarget = Vec3::ToVec3(SceneManager::GetActiveScene()->GetActiveCamera()->GetPos() + SceneManager::GetActiveScene()->GetActiveCamera()->GetTarget());
			Vec3 cameraUp = Vec3::ToVec3(SceneManager::GetActiveScene()->GetActiveCamera()->GetUp());

			SceneManager::GetActiveScene()->GetActiveCamera()->GetFrustum()->setCamDef(cameraPos,cameraTarget,cameraUp);

			//Render children
			for (unsigned i = 0; i < SceneManager::GetActiveScene()->GetChildren().size(); i++) {
				SceneManager::GetActiveScene()->UpdateSceneChildren();
				SceneManager::GetActiveScene()->RenderSceneChildren(renderer, SceneManager::GetActiveScene()->GetActiveCamera()); // Normal draw
			}

			renderer->Render(SceneManager::GetActiveScene()->GetActiveCamera());
		}
	}

	if (!glfwWindowShouldClose(renderer->GetWindow())) { // Check if window should close
		renderer->SwapBuffers(); // Swap buffers
		renderer->PollEvents(); // Poll Events
		renderer->Clear();
	}
	else {
		this->_active = false; // Disable the Core
	}
    
	this->_frames++; // Increment frames by 1
}

float Core::CalculateDeltaTime() {
	static float lastTime = (float)glfwGetTime();
	float currentTime = (float)glfwGetTime();

	float _deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	return _deltaTime;
}

void Core::Destroy() {

	//Delete res manager
	delete ResourceManager::GetInstance();

	//Delete renderer
	if (Core::GetInstance()->renderer) {
		delete Core::GetInstance()->renderer;
		Debug::Log("Renderer deleted", typeid(Core).name());
	}

	delete SceneManager::GetInstance();

	//Exit alut
	SoundManager::Destroy();

	delete Core::GetInstance(); // Delete the instance
	Debug::Log("Core Instance deleted", typeid(Core).name()); // Print Log
}

bool Core::Active() {
	return this->_active; // Return active state
}

void Core::HandleLightRegister(Light* light, int state) {
	if (state == 0) {
		//Register
		renderer->AddLight(light);
	}

	if (state == 1) {
		//remove
		renderer->RemoveLight(light);
	}
}

void Core::SetCursorEnabled(bool state) {
	Core::GetInstance()->renderer->EnableCursor(state);
	Core::GetInstance()->cursorEnabled = state;
}

bool Core::CursorEnabled() {
	return Core::GetInstance()->cursorEnabled;
}

Point2i Core::GetResolution() {
	return Core::GetInstance()->_resolution;
}

GLFWwindow* Core::GetActiveWindow() {
	return Core::GetInstance()->renderer->GetWindow();
}

unsigned Core::GetTimeElapsed() {
	return Core::GetInstance()->_timeElapsed;
}

void Core::SetResolutionReference(Point2i res) {
	Core::GetInstance()->_resolution = res;
}

void Core::SetFov(float amount) {
	if (amount < 0) {
		amount = 0;
	}

	Core::GetInstance()->_fov = amount;
}

float Core::GetFov() {
	return Core::GetInstance()->_fov;
}

SkyBox* Core::GetRendererSkybox() {
	return Core::GetInstance()->renderer->GetSkybox();
}

FrameBuffer* Core::GetRendererFrameBuffer() {
	return Core::GetInstance()->renderer->GetFrameBuffer();
}

glm::mat4 Core::GetRendererViewMatrix() {
	return Core::GetInstance()->renderer->GetViewMatrix();
}

glm::mat4 Core::GetRendererProjectionMatrix() {
	return Core::GetInstance()->renderer->GetProjectionMatrix();
}