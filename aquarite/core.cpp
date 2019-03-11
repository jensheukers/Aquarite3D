/**
*	Filename: core.cpp
*
*	Description: Source file for Core class.
*
*	Version: 28/2/2019
*
*	© 2019, Jens Heukers
*/
#include <Windows.h>
#include <chrono>
#include <sstream>
#include "core.h"
#include "soundmanager.h"
#include "scenemanager.h"
#include "resourcemanager.h"
#include "input.h"
#include "debug.h"
#include "console.h"
#include "luascript.h"
#include "editor.h"

//Native functions for console and lua, these include Run and Spawn, Running a method means running it on this thread,
// We only continue computing if return value is evaluated. If a lua script is spawned, it will be executed on a different
// thread allowing for further computation while the lua script is ran
//Runs a function in lua

//Run a function in lua
std::string Run(std::string value) {
	//Split value
	std::stringstream ss(value);
	std::string segment;
	std::vector<std::string> segments;
	while (std::getline(ss, segment, ' ')) { // Split by space character
		segments.push_back(segment);
	}

	std::vector<std::string> arguments;
	for (size_t i = 2; i < segments.size(); i++) {
		arguments.push_back(segments[i]);
	}
	if (segments.size() > 1) {
		return LuaScript::RunFunction(segments[0], segments[1], arguments);
	}
	return "Lua: No Function Specified!";
}

//We call this function to run threads, we pass state boolean as a reference
void RunThread(std::string value, bool& state) {
	Console::Log(Run(value)); // We run the code
	state = true; // If we get here we know that thread is done running
}

std::string Spawn(std::string value) {
	Thread* thread = new Thread();
	thread->isDone = false;
	thread->thread = std::thread(RunThread, value, std::ref(thread->isDone));

	Core::RegisterThread(thread); // Launch thread, and register
	return "Spawned new thread";
}

std::string DThread(std::string value) {
	return Core::DestroyThread(std::stoi(value));
}

std::string EnableEditor(std::string value) {
	if (value == "0")
		Editor::SetActive(false);
	else if (value == "1")
		Editor::SetActive(true);

	return "";
}

//Native functions for lua, added by default

int Run(lua_State* state) {
	std::string returnValue = Run(lua_tostring(state, -1));
	lua_pushstring(state, returnValue.c_str());
	return 1; // We have pushed 1 value on the lua stack
}

int Spawn(lua_State* state) {
	Spawn(lua_tostring(state, -1)); // Call spawn function
	return 0; // Return 0 because spawn does not return any value
}

//Prints to console from lua
int Lua_ConsoleLog(lua_State* state) {
	Console::Log(lua_tostring(state, -1));
	return 0; // Return nothing
}

//Returns the Core deltatime to Lua
int Lua_GetDeltaTime(lua_State* state) {
	lua_pushnumber(state, Core::GetDeltaTime()); // Push deltatime to stack
	return 1;
}

//Returns the time elapsed to Lua
int Lua_GetTimeElapsed(lua_State* state) {
	lua_pushnumber(state, Core::GetTimeElapsed()); // Push deltatime to stack
	return 1;
}

// Creates a new entity, and adds to scene. Returns a pointer to the object to lua
int Lua_CreateEntity(lua_State* state) {
	if (SceneManager::GetActiveScene()) {
		Entity* entity = new Entity();
		entity->SetModel(ResourceManager::GetModel(lua_tostring(state, -4)));
		entity->position = Vec3((float)lua_tonumber(state, -3), (float)lua_tonumber(state, -2), (float)lua_tonumber(state, -1));

		SceneManager::GetActiveScene()->AddChild(entity);

		//Push to lua stack and return
		lua_pushlightuserdata(state, (void*) entity);

		return 1;
	}

	//If we could not create entity we return nothing
	return 0;
}

//Returns a entity from scene entity children where index matches, Note that we cannot find children using this method
int lua_GetEntityFromScene(lua_State* state) {
	if (SceneManager::GetActiveScene()) {
		lua_pushlightuserdata(state, SceneManager::GetActiveScene()->GetChild((int)lua_tonumber(state, -1)));
		return 1;
	}
	return 0;
}

//Set position of entity referenced by pointer
int lua_SetEntityPosition(lua_State* state) {
	if (SceneManager::GetActiveScene()) {
		Entity* entity = (Entity*) lua_touserdata(state, -4);
		entity->position = Vec3((float)lua_tonumber(state, -3), (float)lua_tonumber(state, -2), (float)lua_tonumber(state, -1));
	}

	return 0;
}

//Returns the position of entity referenced by pointer
int lua_GetEntityPosition(lua_State* state) {
	if (SceneManager::GetActiveScene()) {
		Entity* entity = (Entity*)lua_touserdata(state, -4);
		lua_pushnumber(state, entity->position.x);
		lua_pushnumber(state, entity->position.y);
		lua_pushnumber(state, entity->position.z);
		return 3; // We pushed 3 values onto the stack
	}

	return 0; // If nothing found return 0
}

//Returns the global position of entity referenced by pointer
int lua_GetEntityPositionGlobal(lua_State* state) {
	if (SceneManager::GetActiveScene()) {
		Entity* entity = (Entity*)lua_touserdata(state, -4);
		lua_pushnumber(state, entity->GetPositionGlobal().x);
		lua_pushnumber(state, entity->GetPositionGlobal().y);
		lua_pushnumber(state, entity->GetPositionGlobal().z);
		return 3; // We pushed 3 values onto the stack
	}

	return 0; // If nothing found return 0
}

// Sets the camera position
int Lua_SetCameraPosition(lua_State* state) {
	if (SceneManager::GetActiveScene()) {
		if (SceneManager::GetActiveScene()->GetActiveCamera()) {
			Vec3 pos = Vec3((float)lua_tonumber(state, -3), (float)lua_tonumber(state, -2), (float)lua_tonumber(state, -1));
			SceneManager::GetActiveScene()->GetActiveCamera()->SetPos(pos.ToGLM());
		}
	}

	return 0;
}

// Returns the camera position to lua stack
int Lua_GetCameraPosition(lua_State* state) {
	if (SceneManager::GetActiveScene()) {
		if (SceneManager::GetActiveScene()->GetActiveCamera()) {
			lua_pushnumber(state, SceneManager::GetActiveScene()->GetActiveCamera()->GetPos().x);
			lua_pushnumber(state, SceneManager::GetActiveScene()->GetActiveCamera()->GetPos().y);
			lua_pushnumber(state, SceneManager::GetActiveScene()->GetActiveCamera()->GetPos().z);
			return 3; // We pushed 3 numbers onto the stack
		}
	}

	return 0; // Return 0 if failed
}

//Creates the editor from a lua call
int Lua_EnableEditor(lua_State* state) {
	Editor::SetActive(lua_toboolean(state, -1));
	return 0; // Return nothing
}

void AddNativeFunctionsToLuaStack() {
	//Default methods
	LuaScript::AddNativeFunction("Spawn", Spawn);
	LuaScript::AddNativeFunction("Run", Run);
	LuaScript::AddNativeFunction("ConsoleLog", Lua_ConsoleLog);
	LuaScript::AddNativeFunction("GetDeltaTime", Lua_GetDeltaTime);
	LuaScript::AddNativeFunction("GetTimeElapsed", Lua_GetTimeElapsed);

	//Editor
	LuaScript::AddNativeFunction("EnableEditor", Lua_EnableEditor);

	//Entity methods
	LuaScript::AddNativeFunction("CreateEntity", Lua_CreateEntity);
	LuaScript::AddNativeFunction("GetEntityFromScene", Lua_CreateEntity);
	LuaScript::AddNativeFunction("SetEntityPosition", lua_SetEntityPosition);
	LuaScript::AddNativeFunction("GetEntityPosition", lua_GetEntityPosition);
	LuaScript::AddNativeFunction("GetEntityPositionGlobal", lua_GetEntityPositionGlobal);

	//Camera methods
	LuaScript::AddNativeFunction("SetCameraPosition", Lua_SetCameraPosition);
	LuaScript::AddNativeFunction("GetCameraPosition", Lua_GetCameraPosition);
}

/**
* Main core implementation down here, all methods above are methods that should be able to be called by LUA
*/

//Core implementation
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

	//Initilialize debug (parse renderer)
	Debug::Initialize(renderer);

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

	//If debug is on we should initialize the console, for now it is always enabled
	Console::Initialize();

	//Register default native Lua functions
	AddNativeFunctionsToLuaStack();

	//Add Run / Spawn to console
	Console::AddCommand("run", Run);
	Console::AddCommand("spawn", Spawn);
	Console::AddCommand("destroy", DThread);
	Console::AddCommand("editor", EnableEditor);

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

	//Check threads
	for (size_t t = 0; t < this->threads.size(); t++) {
		if (threads[t]->isDone) { // If thread is done running
			threads[t]->thread.join(); // Join
			
			delete threads[t];
			threads.erase(threads.begin() + t);
			Console::Log("Thread Stopped");
		}
	}

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

			if (Editor::Active()) {
				Editor::Update();
			}
			else {
				//Check if we are not using editor's camera, if we do set it back to the camera that the editor has remembered
				if (SceneManager::GetActiveScene()->GetActiveCamera() == Editor::GetCamera()) {
					SceneManager::GetActiveScene()->SetActiveCamera(Editor::GetActiveCamera());
				}

				//Console is temporary always enabled
				Console::Update();
				Console::Render(renderer);
			}

			Debug::NewFrame();
			renderer->Render(SceneManager::GetActiveScene()->GetActiveCamera());
		}
	}

	//Update Input
	Input::HandleUpdates();

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

void Core::SetRendererDrawFrameBufferToScreen(bool state) {
	Core::GetInstance()->renderer->DrawFrameBufferToScreenObject(state);
}

void Core::RegisterThread(Thread* thread) {
	Core::GetInstance()->threads.push_back(thread);
}

std::string Core::DestroyThread(int index) {
	for (size_t t = 0; t < Core::GetInstance()->threads.size(); t++) {
		if (t == index) {
			Core::GetInstance()->threads[t]->isDone = true;
			return "Marked thread as done";
		}
	}

	return "Could not find thread";
}

void Core::AddToGlobalEntityList(Entity* entity) {
	Core::GetInstance()->entityList.push_back(entity);
}

std::vector<Entity*> Core::GetGlobalEntityList() {
	return Core::GetInstance()->entityList;
}

void Core::RemoveFromGlobalEntityList(Entity* entity) {
	int index = -1;

	for (size_t i = 0; i < GetGlobalEntityList().size(); i++) {
		if (GetGlobalEntityList()[i] == entity) {
			index = i;
		}
	}

	if (index == -1) return;

	Core::GetInstance()->entityList.erase(Core::GetInstance()->entityList.begin() + index);
}