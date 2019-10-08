#include "PanelRender.h"
#include "Color.h"

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

	

	if (ImGui::CollapsingHeader("Mesh Settings")) {
		ImGui::Spacing();
		ImGui::Checkbox(" View Mesh", &App->objects->view_mesh_mode);
		ImGui::SameLine();
		if (ImGui::Checkbox(" Wireframe Mode", &App->objects->wireframe_mode)) {
			App->objects->view_mesh_mode = true;
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50);
		ImGui::SliderInt("Mesh Line Width", &App->objects->mesh_line_width, 1, 30);
		ImGui::Spacing();
		ImGui::ColorEdit3("Mesh Color", (float*)& App->objects->mesh_color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float);
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
