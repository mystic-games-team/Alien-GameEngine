#include "PanelScene.h"
#include "Color.h"
#include "imgui/imgui.h"
#include "ResourceTexture.h"
#include "imgui/imgui_internal.h"
#include "FileNode.h"
#include "PanelSceneSelector.h"

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
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
		ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);
	}
	else
		ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

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

		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DROP_ID_PROJECT_NODE, ImGuiDragDropFlags_SourceNoDisableHover);
		if (payload != nullptr && payload->IsDataType(DROP_ID_PROJECT_NODE)) {
			FileNode* node = *(FileNode**)payload->Data;

			// drop texture
			if (node != nullptr && node->type == FileDropType::TEXTURE && App->objects->GetSelectedObject() != nullptr) {
				std::string path = App->file_system->GetPathWithoutExtension(node->path + node->name);
				path += "_meta.alien";

				u64 ID = App->resources->GetIDFromAlienPath(path.data());

				ResourceTexture* texture_dropped = (ResourceTexture*)App->resources->GetResourceWithID(ID);

				if (texture_dropped != nullptr) {
					App->importer->ApplyTextureToSelectedObject(texture_dropped);
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
				std::string full_scene_path = node->path + node->name;
				App->ui->panel_scene_selector->LoadSceneWithPath(full_scene_path.data());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::End();
}
