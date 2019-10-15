#include "PanelConsole.h"

PanelConsole::PanelConsole(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra) :
	Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Panel Console", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
	enabled = true;
}

PanelConsole::~PanelConsole()
{
}

void PanelConsole::PanelLogic()
{

	ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);
	
	ImGui::TextUnformatted(App->log_string.begin());

	if (scroll_x) {
		scroll_x = false;
		ImGui::SetScrollHereX(1.0F);
	}
	if (scroll_y) {
		scroll_y = false;
		ImGui::SetScrollHereY(1.0F);
	}

	if (ImGui::GetScrollY() / ImGui::GetScrollMaxY() >= 0.95F) {
		ImGui::SetScrollHereY(1.0F);
	}

	ImGui::End();

	
}

void PanelConsole::OnPanelDesactive()
{
	scroll_x = true;
	scroll_y = true;
}
