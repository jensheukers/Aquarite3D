/**
*	Filename: main.cpp
*
*	Description: DESCRIPTION HERE
*
*	Version: DATE
*
*	© 2018, YOUR COMPANY NAME
*/
#include "../aquarite/core.h"
#include "../aquarite/debug.h"
#include "../aquarite/scenemanager.h"
#include "../aquarite/input.h"
#include "../aquarite/ui/uielement.h"

int main(int argc, char* argv[]) {
	Core::GetInstance()->Initialize(argv, Point2i(1280,720)); //Initialize Core

	//Load resources
	Meta* meta = new Meta();
	ResourceManager::LoadMeta("res/example.meta");

	Scene* scene = new Scene();
	scene->LoadSceneData("res/example/scene.ascene");
	SceneManager::AddScene(scene);
	SceneManager::SetActiveScene(0);

	FPSCamera* camera = new FPSCamera();
	scene->SetActiveCamera(camera);
	
	Core::SetCursorEnabled(false);
	
	UIElement* element = new UIElement();

	while (Core::GetInstance()->Active()) { // While the core is still active
		camera->OnMouseMovement(Input::GetMousePosition().x, Input::GetMousePosition().y);

		if (Input::GetKey(KEYCODE_W)) {
			camera->Move(CameraDirection::Forward, Core::GetDeltaTime() * 5);
		}

		if (Input::GetKey(KEYCODE_S)) {
			camera->Move(CameraDirection::Backward, Core::GetDeltaTime() * 5);
		}

		if (Input::GetKey(KEYCODE_A)) {
			camera->Move(CameraDirection::Left, Core::GetDeltaTime() * 5);
		}

		if (Input::GetKey(KEYCODE_D)) {
			camera->Move(CameraDirection::Right, Core::GetDeltaTime() * 5);
		}

		if (Input::GetKeyDown(KEYCODE_ESCAPE)) {
			Core::SetCursorEnabled(true);
		}

		Core::GetInstance()->HandleUpdates(); // Handle the updates
	}
	 // Destroy Core Instance
	Core::GetInstance()->Destroy();
}