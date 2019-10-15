#include "PanelScene.h"
#include "Color.h"
#include "imgui/imgui.h"

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
	ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse);

	ImGui::Image((ImTextureID)App->renderer3D->GetSceneTexture()->id, { ImGui::GetWindowWidth(),ImGui::GetWindowHeight() });

	ImGui::End();
}
