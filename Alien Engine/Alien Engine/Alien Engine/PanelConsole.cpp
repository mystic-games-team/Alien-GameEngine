#include "PanelConsole.h"

PanelConsole::PanelConsole(const std::string & panel_name, const std::vector<SDL_Scancode>& shortcuts) : Panel(panel_name, shortcuts)
{
}

PanelConsole::~PanelConsole()
{
}

void PanelConsole::PanelLogic()
{
	
	ImGui::Begin("Configuration", &enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);
	
	ImGui::TextUnformatted(App->log_string.begin());

	ImGui::End();

	
}
