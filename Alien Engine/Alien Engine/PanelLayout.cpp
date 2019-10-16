#include "PanelLayout.h"

PanelLayout::PanelLayout(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Edit Layout", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
}

PanelLayout::~PanelLayout()
{
}

void PanelLayout::PanelLogic()
{
	ImGui::OpenPopup(panel_name.c_str());
	ImGui::SetNextWindowContentWidth(300);
	if (ImGui::BeginPopupModal(panel_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
	{




		ImGui::EndPopup();
	}


}
