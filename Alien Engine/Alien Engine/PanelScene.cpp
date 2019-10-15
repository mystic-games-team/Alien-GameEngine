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
	ImGui::SetNextWindowSize({ 800,600 });
	ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoScrollbar|ImGuiDockNodeFlags_NoResize|ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoScrollWithMouse);
	App->camera->is_scene_hovered = ImGui::IsWindowHovered();
	App->camera->is_scene_focused = ImGui::IsWindowFocused();
	ImGui::Text("");
	ImGui::SameLine(ImGui::GetWindowWidth() - 800);
	ImGui::Image((ImTextureID)App->renderer3D->tex->id, { 800,600 }, { 0,1 }, { 1,0 });
	ImGui::End();
}
