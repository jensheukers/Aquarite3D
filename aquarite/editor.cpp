//  Main source file for editor class
//
//	Version: 15/1/2019
//
//	© 2019, Jens Heukers
#include "editor.h"
#include "core.h"
#include "input.h"
#include "scenemanager.h"

//ImGui global style settings
static glm::vec3 color_for_text = glm::vec3(236.f / 255.f, 240.f / 255.f, 241.f / 255.f);
static glm::vec3 color_for_head = glm::vec3(41.f / 255.f, 128.f / 255.f, 185.f / 255.f);
static glm::vec3 color_for_area = glm::vec3(57.f / 255.f, 79.f / 255.f, 105.f / 255.f);
static glm::vec3 color_for_body = glm::vec3(44.f / 255.f, 62.f / 255.f, 80.f / 255.f);
static glm::vec3 color_for_pops = glm::vec3(33.f / 255.f, 46.f / 255.f, 60.f / 255.f);

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

	if (Input::GetButton(BUTTONCODE_RIGHT)) {
		Core::SetCursorEnabled(false);
		instance->camera->OnMouseMovement(Input::GetMousePosition().x, Input::GetMousePosition().y);
	}
	else {
		Core::SetCursorEnabled(true);
	}
	
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


	//Handle editor updates
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
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

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