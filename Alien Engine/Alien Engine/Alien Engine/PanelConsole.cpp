#include "PanelConsole.h"

PanelConsole::PanelConsole(const std::string & panel_name, const std::vector<SDL_Scancode>& shortcuts) : Panel(panel_name, shortcuts)
{
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
