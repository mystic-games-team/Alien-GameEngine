#include "PanelScene.h"
#include "Color.h"
#include "imgui/imgui.h"
#include "ResourceTexture.h"
#include "imgui/imgui_internal.h"

PanelScene::PanelScene(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Panel Scene", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
	enabled = true;
}

PanelScene::~PanelScene()
{
}

void PanelScene::PanelLogic()
{
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
		ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);
	}
	else
		ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	
	if (App->input->GetKey(SDL_SCANCODE_DELETE) && App->objects->GetSelectedObject() != nullptr && ImGui::IsWindowFocused())
	{
		App->objects->GetSelectedObject()->ToDelete();
	}

	App->camera->is_scene_hovered = ImGui::IsWindowHovered();
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
	
	ImGui::Image((ImTextureID)App->renderer3D->tex->id, { width,height }, { 0,1 }, { 1,0 });

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

		// drop texture
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DROP_ID_TEXTURE, ImGuiDragDropFlags_SourceNoDisableHover);
		if (payload != nullptr && payload->IsDataType(DROP_ID_TEXTURE)) {
			FileNode* node = *(FileNode**)payload->Data;
			if (node != nullptr) {
				//App->objects->ReparentGameObject(obj, App->objects->base_game_object);
			}
		}

		// drop model
		payload = ImGui::AcceptDragDropPayload(DROP_ID_MODEL, ImGuiDragDropFlags_SourceNoDisableHover);
		if (payload != nullptr && payload->IsDataType(DROP_ID_MODEL)) {
			FileNode* node = *(FileNode**)payload->Data;
			if (node != nullptr) {
				std::string meta_path = LIBRARY_MODELS_FOLDER + App->file_system->GetBaseFileName(node->name.data()) + ".alien";
				App->resources->CreateNewModelInstanceOf(meta_path.data());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::End();
}
