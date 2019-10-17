#include "PanelScene.h"
#include "Color.h"
#include "imgui/imgui.h"

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
	ImGui::SetNextWindowSize({ 960,540 });
	ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoScrollbar| ImGuiWindowFlags_NoScrollWithMouse);

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
	ImGui::End();
}
