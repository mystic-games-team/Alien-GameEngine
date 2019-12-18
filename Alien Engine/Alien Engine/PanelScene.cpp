#include "PanelScene.h"
#include "Color.h"
#include "imgui/imgui.h"
#include "ResourceTexture.h"
#include "imgui/imgui_internal.h"
#include "FileNode.h"
#include "PanelSceneSelector.h"
#include "ResourcePrefab.h"
#include "ComponentTransform.h"
#include "ReturnZ.h"
#include "PanelGame.h"

PanelScene::PanelScene(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Panel Scene", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
}

PanelScene::~PanelScene()
{
}

void PanelScene::PanelLogic()
{
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT || ImGuizmo::IsOver()) {
		ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);
	}
	else
		ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	App->camera->is_scene_focused = ImGui::IsWindowFocused();

	if (height > ImGui::GetWindowHeight())
	{
		height = ImGui::GetWindowHeight();
		width = (height * 16) / 9;
	}
	else if ((width > ImGui::GetWindowWidth()) || (width > ImGui::GetWindowWidth() && height > ImGui::GetWindowHeight()))
	{
		width = ImGui::GetWindowWidth();
		height = (width * 9) / 16;
	}
	else if((width < ImGui::GetWindowWidth() && height < ImGui::GetWindowHeight()))
	{
		if ((ImGui::GetWindowHeight()-lastHeight)!=0)
		{
			height = ImGui::GetWindowHeight();
			width = (height * 16) / 9;
		}
		else
		{
			width = ImGui::GetWindowWidth();
			height = (width * 9) / 16;
		}
	}

	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - width) * 0.5f);
	ImGui::SetCursorPosY((ImGui::GetWindowHeight() - height) * 0.5f);

	posX = ImGui::GetWindowPos().x + ImGui::GetCursorPosX();
	posY = ImGui::GetWindowPos().y + ImGui::GetCursorPosY();

	ImGui::Image((ImTextureID)App->renderer3D->scene_tex->id, { width,height }, { 0,1 }, { 1,0 });
	App->camera->is_scene_hovered = ImGui::IsItemHovered();

	lastHeight = ImGui::GetWindowHeight();


	// drop a fbx/texture in the window
	ImVec2 min_space = ImGui::GetWindowContentRegionMin();
	ImVec2 max_space = ImGui::GetWindowContentRegionMax();

	min_space.x += ImGui::GetWindowPos().x;
	min_space.y += ImGui::GetWindowPos().y;
	max_space.x += ImGui::GetWindowPos().x;
	max_space.y += ImGui::GetWindowPos().y;

	// drop project files
	if (ImGui::BeginDragDropTargetCustom({ min_space.x,min_space.y, max_space.x,max_space.y }, ImGui::GetID(panel_name.data()))) {

		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DROP_ID_PROJECT_NODE, ImGuiDragDropFlags_SourceNoDisableHover);
		if (payload != nullptr && payload->IsDataType(DROP_ID_PROJECT_NODE)) {
			FileNode* node = *(FileNode**)payload->Data;

			// drop texture
			if (node != nullptr && node->type == FileDropType::TEXTURE && !App->objects->GetSelectedObjects().empty()) {
				std::string path = App->file_system->GetPathWithoutExtension(node->path + node->name);
				path += "_meta.alien";

				u64 ID = App->resources->GetIDFromAlienPath(path.data());

				ResourceTexture* texture_dropped = (ResourceTexture*)App->resources->GetResourceWithID(ID);

				if (texture_dropped != nullptr) {
					App->importer->ApplyTextureToSelectedObject(texture_dropped);
				}
			}

			// drop prefab
			if (node != nullptr && node->type == FileDropType::PREFAB) {
				std::string path = App->file_system->GetPathWithoutExtension(node->path + node->name) + ".alienPrefab";
				u64 ID = App->resources->GetIDFromAlienPath(path.data());
				if (ID != 0) {
					ResourcePrefab* prefab = (ResourcePrefab*)App->resources->GetResourceWithID(ID);
					prefab->ConvertToGameObjects(App->objects->GetRoot(false));
					ReturnZ::AddNewAction(ReturnZ::ReturnActions::ADD_OBJECT, App->objects->GetRoot(false)->children.back());
				}
			}

			// drop model
			if (node != nullptr && node->type == FileDropType::MODEL3D) {
				std::string path = App->file_system->GetPathWithoutExtension(node->path + node->name);
				path += "_meta.alien";

				u64 ID = App->resources->GetIDFromAlienPath(path.data());

				std::string meta_path = LIBRARY_MODELS_FOLDER + std::to_string(ID) + ".alienModel";

				if (!App->resources->CreateNewModelInstanceOf(meta_path.data())) { // if it goes here it is because this file wasn't imported yet, so import it now

					App->importer->LoadModelFile(std::string(node->path + node->name).data());
					ID = App->resources->GetIDFromAlienPath(path.data());
					meta_path = LIBRARY_MODELS_FOLDER + std::to_string(ID) + ".alienModel";
					App->resources->CreateNewModelInstanceOf(meta_path.data());
				}
			}

			// drop scene
			if (node != nullptr && node->type == FileDropType::SCENE) {
				static char curr_dir[MAX_PATH];
				GetCurrentDirectoryA(MAX_PATH, curr_dir);
				std::string full_scene_path = curr_dir + std::string("/") + node->path + node->name;
				App->ui->panel_scene_selector->LoadSceneWithPath(full_scene_path.data());
			}
		}

		ImGui::EndDragDropTarget();
	}

	GuizmosControls();
	GuizmosLogic();

	ImDrawList* list = ImGui::GetWindowDrawList();
	if (list != nullptr && list->CmdBuffer.size() > 1)
		is_window_being_rendered = true;
	else
		is_window_being_rendered = false;

	if (App->renderer3D->selected_game_camera != nullptr && is_window_being_rendered)
	{
		ImGui::SetNextWindowPos(ImVec2(max_space.x - 212, max_space.y - 154));
		ImGui::SetNextWindowSize(ImVec2(192, 134));
		ImGui::Begin("Camera Selected Preview", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing);

		if (App->renderer3D->actual_game_camera != App->renderer3D->selected_game_camera)
			ImGui::Image((ImTextureID)App->renderer3D->sc_game_tex->id, { 176,99 }, { 0,1 }, { 1,0 });
		else
			ImGui::Image((ImTextureID)App->renderer3D->game_tex->id, { 176,99 }, { 0,1 }, { 1,0 });

		ImGui::End();
	}

	

	ImGui::End();
}

void PanelScene::GuizmosLogic()
{
	// TODO: gizmo
	//if (App->objects->GetSelectedObjects() != nullptr) {
	//	ComponentTransform* transform = (ComponentTransform*)App->objects->GetSelectedObjects()->GetComponent(ComponentType::TRANSFORM);

	//	float4x4 view_transposed = App->camera->fake_camera->frustum.ViewMatrix();
	//	view_transposed.Transpose();
	//	float4x4 projection_transposed = App->camera->fake_camera->frustum.ProjectionMatrix();
	//	projection_transposed.Transpose();
	//	float4x4 object_transform_matrix = transform->global_transformation;
	//	object_transform_matrix.Transpose();
	//	float4x4 delta_matrix;

	//	ImGuizmo::SetRect(posX, posY, width, height);
	//	ImGuizmo::SetDrawlist();
	//	ImGuizmo::Manipulate(view_transposed.ptr(), projection_transposed.ptr(), guizmo_operation, guizmo_mode, object_transform_matrix.ptr(), delta_matrix.ptr());
	//	static bool guizmo_return = true;
	//	
	//	if (!ImGui::IsAnyPopupActive() && ImGuizmo::IsUsing() && !transform->game_object_attached->is_static)
	//	{
	//		if (guizmo_return) {
	//			ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, transform);
	//			guizmo_return = false;
	//		}
	//		ComponentTransform* parent_transform = (ComponentTransform*)App->objects->GetSelectedObjects()->parent->GetComponent(ComponentType::TRANSFORM);
	//		if (App->objects->GetSelectedObjects()->parent != App->objects->GetRoot(true))
	//		{
	//			transform->SetGlobalTransformation(parent_transform->global_transformation.Inverted() * object_transform_matrix.Transposed());
	//		}
	//		else
	//			transform->SetGlobalTransformation(object_transform_matrix.Transposed());
	//	}
	//	else if (!guizmo_return) {
	//		guizmo_return = true;
	//	}
	//}
}

void PanelScene::GuizmosControls()
{
	if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) && (App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_REPEAT)&& (App->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT))
	{
		guizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		guizmo_operation = ImGuizmo::OPERATION::ROTATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		guizmo_operation = ImGuizmo::OPERATION::SCALE;
	}
	if ((App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) && (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) && (App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_REPEAT))
	{
		guizmo_mode = ImGuizmo::MODE::WORLD;
	}
	if ((App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) && (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN))
	{
		guizmo_mode = ImGuizmo::MODE::LOCAL;
	}
}