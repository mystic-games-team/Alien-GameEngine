#include "PanelRender.h"

PanelRender::PanelRender(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Panel Render", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
}

PanelRender::~PanelRender()
{
}

void PanelRender::PanelLogic()
{
	ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse);

	if (ImGui::Checkbox("##CheckBox", &App->objects->view_mesh_mode));

	if (ImGui::CollapsingHeader("Mesh Settings")) {

	}
	if (ImGui::CollapsingHeader("Background Color")) {

	}
	if (ImGui::CollapsingHeader("Personalize Grid")) {

	}
	if (ImGui::CollapsingHeader("Vertex Normals")) {

	}
	if (ImGui::CollapsingHeader("Face Normals")) {

	}
	ImGui::End();
}
