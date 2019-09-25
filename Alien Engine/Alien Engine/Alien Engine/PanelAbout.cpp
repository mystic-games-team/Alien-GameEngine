#include "PanelAbout.h"

PanelAbout::PanelAbout(const std::string& panel_name, const std::vector<SDL_Scancode>& shortcuts) : Panel(panel_name, shortcuts)
{
}

PanelAbout::~PanelAbout()
{
}

void PanelAbout::PanelLogic()
{
	ImGui::Begin("TestMenu", (bool*)0, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Close", "Ctrl+W")) {
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}
