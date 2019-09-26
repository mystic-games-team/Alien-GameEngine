#include "PanelConfig.h"
#include "ModuleWindow.h"

PanelConfig::PanelConfig(const std::string& panel_name, const std::vector<SDL_Scancode>& shortcuts) : Panel(panel_name, shortcuts)
{
}

PanelConfig::~PanelConfig()
{
}

void PanelConfig::PanelLogic()
{
	ImGui::Begin("Configuration", &enabled, ImGuiWindowFlags_NoCollapse);

	if (ImGui::CollapsingHeader("Application"))
	{
	}
	if (ImGui::CollapsingHeader("Window"))
	{
		if (ImGui::SliderInt(" Width", &App->window->width, 0, 1920)) {
			SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
		}
		if (ImGui::SliderInt(" Height", &App->window->height, 0, 1080)) {
			SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
		}
	}

	
	ImGui::End();

}
