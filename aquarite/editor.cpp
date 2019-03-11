//  Main source file for editor class
//
//	Version: 4/3/2019
//
//	© 2019, Jens Heukers
#include "debug.h"
#include "editor.h"
#include "core.h"
#include "input.h"
#include "luascript.h"
#include "scenemanager.h"
#include "graphics/light.h"

//ImGui global style settings
static glm::vec3 color_for_text = glm::vec3(236.f / 255.f, 240.f / 255.f, 241.f / 255.f);
static glm::vec3 color_for_head = glm::vec3(41.f / 255.f, 128.f / 255.f, 185.f / 255.f);
static glm::vec3 color_for_area = glm::vec3(57.f / 255.f, 79.f / 255.f, 105.f / 255.f);
static glm::vec3 color_for_body = glm::vec3(44.f / 255.f, 62.f / 255.f, 80.f / 255.f);
static glm::vec3 color_for_pops = glm::vec3(33.f / 255.f, 46.f / 255.f, 60.f / 255.f);

static int _currentEntityItem = 0;
static int _lastEntityItem = 0;

glm::vec3 activeCameraOriginPos;
glm::vec3 activeCameraOriginTarget;

Editor* Editor::instance;

Editor* Editor::GetInstance() {
	if (!instance) {
		instance = new Editor();
		instance->Initialize();
	}
	return instance;
}

void Editor::HandleLoadMenu() {
	ImGui::Begin("Load", &this->loadMenuActive);

	static char buffer[75];
	ImGui::InputText("Path (from application root)", buffer, sizeof(buffer));
	if (ImGui::Button("Load")) {
		delete SceneManager::GetActiveScene();
		Scene* scene = new Scene();
		scene->LoadSceneData(buffer);
		scene->SetActiveCamera(instance->camera);
		int id = SceneManager::AddScene(scene);
		SceneManager::SetActiveScene(id);
	}
	ImGui::End();
}

void Editor::HandleSaveMenu() {
	ImGui::Begin("Save", &this->loadMenuActive);
	ImGui::End();
}

void Editor::HandleEntityInfoMenu() {
	ImGui::Begin("Entity Info", &this->entityInfoActive);
	if (currentSelection) {
		//Position/Rotation/Scale
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Transform Info");

		float _currentEntityPos[3] = { currentSelection->position.x, currentSelection->position.y, currentSelection->position.z };
		float _currentEntityRot[3] = { currentSelection->GetRotation().x, currentSelection->GetRotation().y, currentSelection->GetRotation().z };;
		float _currentEntityScale[3] = { currentSelection->GetScale().x, currentSelection->GetScale().y, currentSelection->GetScale().z };;


		ImGui::DragFloat3("position", _currentEntityPos);
		ImGui::DragFloat3("rotation", _currentEntityRot);
		ImGui::DragFloat3("scale", _currentEntityScale);

		currentSelection->position = Vec3(_currentEntityPos[0], _currentEntityPos[1], _currentEntityPos[2]);
		currentSelection->SetRotation(Vec3(_currentEntityRot[0], _currentEntityRot[1], _currentEntityRot[2]));
		currentSelection->SetScale(Vec3(_currentEntityScale[0], _currentEntityScale[1], _currentEntityScale[2]));

		//Parent Info
		ImGui::Text("");
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Parent info");
		std::string _parentNameString = "Parent name: ";
		_parentNameString.append(currentSelection->GetParent()->GetName());
		ImGui::Text(_parentNameString.c_str());

		std::string _parentIdString = "Parent id: ";
		_parentIdString.append(std::to_string(currentSelection->GetParent()->GetId()));
		ImGui::Text(_parentIdString.c_str());

		//Check if light
		if (dynamic_cast<Light*>(currentSelection)) {
			Light* light = dynamic_cast<Light*>(currentSelection);
			DirectionalLight* dirLight;

			ImGui::Text("");
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Light Properties");

			float _lightAmbientColor[4] = { light->GetAmbient().x, light->GetAmbient().y, light->GetAmbient().z, 1 };
			float _lightDiffuseColor[4] = { light->GetDiffuse().x, light->GetDiffuse().y, light->GetDiffuse().z, 1 };
			float _lightSpecularColor[4] = { light->GetSpecular().x, light->GetSpecular().y, light->GetSpecular().z, 1 };


			ImGui::ColorEdit4("Ambient", _lightAmbientColor);
			ImGui::ColorEdit4("Diffuse", _lightDiffuseColor);
			ImGui::ColorEdit4("Specular", _lightSpecularColor);


			light->SetAmbient(Vec3(_lightAmbientColor[0], _lightAmbientColor[1], _lightAmbientColor[2]).ToGLM());
			light->SetDiffuse(Vec3(_lightDiffuseColor[0], _lightDiffuseColor[1], _lightDiffuseColor[2]).ToGLM());
			light->SetSpecular(Vec3(_lightSpecularColor[0], _lightSpecularColor[1], _lightSpecularColor[2]).ToGLM());

			if (light->GetLightType() == LightType::Directional) {
				dirLight = dynamic_cast<DirectionalLight*>(light);
				float _dirLightDirection[3] = { dirLight->GetDirection().x, dirLight->GetDirection().y, dirLight->GetDirection().z };
				ImGui::DragFloat3("direction", _dirLightDirection);
				dirLight->SetDirection(Vec3(_dirLightDirection[0], _dirLightDirection[1], _dirLightDirection[2]).ToGLM());
			}
		}
		else {
			//Model Info
			ImGui::Text("");
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Model name");

			if (currentSelection->GetModel()) {
				static char _currentEntityModelName[20];
				if (_currentEntityItem != _lastEntityItem) {
					std::strcpy(_currentEntityModelName, currentSelection->GetModel()->GetName().c_str());
				}

				ImGui::PushItemWidth(-1);
				ImGui::InputText("", _currentEntityModelName, sizeof(_currentEntityModelName));

				if (ImGui::Button("Apply")) {
					if (ResourceManager::GetModel(_currentEntityModelName)) {
						currentSelection->SetModel(ResourceManager::GetModel(_currentEntityModelName));
					}
					else {
						Debug::Log("Invalid name", "main()");
					}
				}

				//Info
				std::string _amountMeshesString = "Amount of meshes: ";
				_amountMeshesString.append(std::to_string(currentSelection->GetModel()->GetMeshesCount()));
				ImGui::Text(_amountMeshesString.c_str());

				std::string _amountMaterialsString = "Amount of materials: ";
				_amountMaterialsString.append(std::to_string(currentSelection->GetModel()->GetMaterialCount()));
				ImGui::Text(_amountMaterialsString.c_str());

				unsigned _amountVertices = 0;
				for (int i = 0; i < currentSelection->GetModel()->GetMeshesCount(); i++) {
					_amountVertices += currentSelection->GetModel()->GetMesh(i)->GetVerticesCount();
				}

				std::string _amountVerticesString = "Amount of vertices: ";
				_amountVerticesString.append(std::to_string(_amountVertices));
				ImGui::Text(_amountVerticesString.c_str());

				std::string _amountTrianglesString = "Amount of triangles: ";
				_amountTrianglesString.append(std::to_string(_amountVertices / 3));
				ImGui::Text(_amountTrianglesString.c_str());
			}
			else {
				ImGui::Text("Entity has no model");
				if (ImGui::Button("Add Model")) {
					currentSelection->SetModel(new Model());
				}
			}
		}
	}
	else {
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "No entity selected...");
	}

	ImGui::End();
}

void Editor::HandleEntityListMenu() {
	ImGui::Begin("Entity List", &this->entityListActive); 

	//Entities
	std::vector<std::string> _entitiesNames;

	for (size_t i = 0; i < SceneManager::GetActiveScene()->GetChildren().size(); i++) {
		std::string entName = std::to_string(SceneManager::GetActiveScene()->GetChild(i)->GetId());
		entName.append(" - ");
		entName.append(SceneManager::GetActiveScene()->GetChild(i)->GetName());
		_entitiesNames.push_back(entName);
	}

	//Why I have to do this im not sure but it works
	std::vector<const char*> _convertedEntitiesNames;
	for (size_t i = 0; i < _entitiesNames.size(); i++) {
		_convertedEntitiesNames.push_back(_entitiesNames[i].c_str());
	}

	ImGui::TextColored(ImVec4(1, 0, 0, 1), "Entities");
	ImGui::PushItemWidth(-1);
	ImGui::ListBox("", &_currentEntityItem, _convertedEntitiesNames.data(), SceneManager::GetActiveScene()->GetChildren().size(), 10);

	if (SceneManager::GetActiveScene()->GetChildren().size() > 0) {
		currentSelection = SceneManager::GetActiveScene()->GetChild(_currentEntityItem);
	}

	if (ImGui::Button("Create New")) {
		Entity* entity = new Entity();
		entity->position = Vec3(camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);
		SceneManager::GetActiveScene()->AddChild(entity);
		_currentEntityItem = SceneManager::GetActiveScene()->GetChildren().size() - 1;
	}
	ImGui::SameLine(0);
	if (ImGui::Button("Copy")) {
		Entity* copyEntity = new Entity();
		copyEntity->position = currentSelection->position;
		copyEntity->SetRotation(currentSelection->GetRotation());
		copyEntity->SetScale(currentSelection->GetScale());
		copyEntity->SetModel(currentSelection->GetModel());
		SceneManager::GetActiveScene()->AddChild(copyEntity);
		_currentEntityItem = SceneManager::GetActiveScene()->GetChildren().size() - 1;
	}
	ImGui::SameLine(0);
	if (ImGui::Button("Delete")) {
		if (currentSelection) {
			if (dynamic_cast<Light*>(currentSelection)) {
				SceneManager::GetActiveScene()->RemoveLight(dynamic_cast<Light*>(currentSelection));
			}
			else {
				SceneManager::GetActiveScene()->RemoveChild(currentSelection);
			}
			delete currentSelection; // erase from mem
			currentSelection = nullptr;
		}
	}

	ImGui::End();
}

void Editor::HandleScriptMenu() {
	ImGui::Begin("Lua Script Console", &this->scriptConsoleActive);
	static char buffer[64] = "function main(args)\n return \"\" \n end";
	ImGui::InputTextMultiline("", buffer, 1024, ImVec2(512, 256));

	static std::string output = "";

	if (ImGui::Button("Run on Lua")) {
		LuaScript::Run(buffer);
		std::vector<std::string> args;
		output = LuaScript::RunFunction("main", args);
	}

	ImGui::SameLine();

	ImGui::Text("function main() is called by default when \"Run on Lua button\" is pressed");
	ImGui::Text("Output: ");
	ImGui::SameLine();
	ImGui::Text(output.c_str());


	ImGui::End();
}

void Editor::AddPointLight() {
	Light* light = new Light();
	light->SetLightType(LightType::PointLight);
	light->position = Vec3(camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);
	SceneManager::GetActiveScene()->AddLight(light);
}

void Editor::AddDirectionalLight() {
	DirectionalLight* light = new DirectionalLight();
	light->SetName("Directional Light");
	light->position = Vec3(camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);
	SceneManager::GetActiveScene()->AddLight(light);
}

void Editor::Initialize() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.GrabRounding = 0.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

	style.Colors[ImGuiCol_Text] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.95f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.58f);
	style.Colors[ImGuiCol_Border] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.21f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.15f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.73f);

	instance->camera = new FPSCamera();
}

void Editor::Update() {
	if (SceneManager::GetActiveScene()->GetActiveCamera() != instance->camera) {
		SceneManager::GetActiveScene()->SetActiveCamera(instance->camera);
	}

	//Handle camera movement
	instance->activeCamera->SetPos(activeCameraOriginPos);
	instance->activeCamera->SetTarget(activeCameraOriginTarget);

	if (Input::GetButton(BUTTONCODE_LEFT) && !Core::CursorEnabled()) {
		if (instance->currentSelection) {
			instance->currentSelection->position = Input::GetMouseRayPositionWorldSpace(instance->camera, 5.0f);
		}
	}

	if (Input::GetButton(BUTTONCODE_RIGHT)) {
		Core::SetCursorEnabled(false);
		instance->camera->OnMouseMovement(Input::GetMousePosition().x, Input::GetMousePosition().y);

		if (Input::GetKey(KEYCODE_W)) {
			instance->camera->Move(CameraDirection::Forward, Core::GetDeltaTime() * 5);
		}

		if (Input::GetKey(KEYCODE_S)) {
			instance->camera->Move(CameraDirection::Backward, Core::GetDeltaTime() * 5);
		}

		if (Input::GetKey(KEYCODE_A)) {
			instance->camera->Move(CameraDirection::Left, Core::GetDeltaTime() * 5);
		}

		if (Input::GetKey(KEYCODE_D)) {
			instance->camera->Move(CameraDirection::Right, Core::GetDeltaTime() * 5);
		}
	}
	else {
		Core::SetCursorEnabled(true);
	}

	//Handle editor updates

	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[KEYCODE_BACKSPACE] = Input::GetKey(KEYCODE_BACKSPACE);
	io.KeysDown[KEYCODE_ENTER] = Input::GetKeyDown(KEYCODE_ENTER);
	io.KeysDown[KEYCODE_DOWN] = Input::GetKeyDown(KEYCODE_DOWN);
	io.KeysDown[KEYCODE_UP] = Input::GetKeyDown(KEYCODE_UP);
	io.KeysDown[KEYCODE_LEFT] = Input::GetKeyDown(KEYCODE_LEFT);
	io.KeysDown[KEYCODE_RIGHT] = Input::GetKeyDown(KEYCODE_RIGHT);

	//Begin with title bar
	ImGui::Begin("Editor", &instance->active, ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowPos(ImVec2(0,0));
	ImGui::SetWindowSize(ImVec2(Core::GetResolution().x, 30));

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene")) {
				delete SceneManager::GetActiveScene();
				Scene* scene = new Scene();
				scene->SetActiveCamera(instance->camera);
				int id = SceneManager::AddScene(scene);
				SceneManager::SetActiveScene(id);
			}

			if (ImGui::MenuItem("Save Scene")) {
				instance->saveMenuActive = true;
			}

			if (ImGui::MenuItem("Load Scene")) {
				instance->loadMenuActive = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Entity Info")) {
				instance->entityInfoActive = true;
			}

			if (ImGui::MenuItem("Entity List")) {
				instance->entityListActive = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Light"))
		{
			if (ImGui::MenuItem("Add Point Light")) { instance->AddPointLight(); }
			if (ImGui::MenuItem("Add Directional Light")) { instance->AddDirectionalLight(); }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug")) {
			if (ImGui::MenuItem("Script Console")) { instance->scriptConsoleActive = true; }
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	//Submenu handling
	if (instance->loadMenuActive) 
		instance->HandleLoadMenu();
	if (instance->saveMenuActive)
		instance->HandleSaveMenu();
	if (instance->entityInfoActive)
		instance->HandleEntityInfoMenu();
	if (instance->entityListActive)
		instance->HandleEntityListMenu();
	if (instance->scriptConsoleActive)
		instance->HandleScriptMenu();

	ImGui::End();
}

bool Editor::Active() {
	return Editor::GetInstance()->active;
}

void Editor::SetActive(bool state) {
	Editor::GetInstance()->active = state;
	Editor::GetInstance()->activeCamera = SceneManager::GetActiveScene()->GetActiveCamera();
	activeCameraOriginPos = SceneManager::GetActiveScene()->GetActiveCamera()->GetPos();
	activeCameraOriginTarget = SceneManager::GetActiveScene()->GetActiveCamera()->GetTarget();
}

Camera* Editor::GetCamera() {
	return GetInstance()->camera;
}

Camera* Editor::GetActiveCamera() {
	return GetInstance()->activeCamera;
}