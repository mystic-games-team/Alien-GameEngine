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

	

	if (ImGui::CollapsingHeader("Mesh Settings")) 
	{
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
	if (ImGui::CollapsingHeader("Background")) 
	{
		ImGui::Spacing();
		ImGui::ColorEdit3("Background Color", (float*)&App->renderer3D->background_color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float);
		ImGui::Spacing();
	}
	if (ImGui::CollapsingHeader("Personalize Grid")) 
	{
		ImGui::Spacing();
		ImGui::Checkbox("View Grid", &App->objects->allow_grid);
		ImGui::InputFloat("Spacing", &App->renderer3D->grid_spacing, 0.2, 1, 2);
		if (App->renderer3D->grid_spacing < 0.2)
		{
			App->renderer3D->grid_spacing = 0.2;
		}
		ImGui::InputInt("Grid Length", &App->renderer3D->length_grid,1,100);
		if (App->renderer3D->length_grid < 1)
		{
			App->renderer3D->length_grid = 1;
		}
		ImGui::ColorEdit3("Grid Color", (float*)&App->renderer3D->grid_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_Float);
		ImGui::SliderInt("Line Width", &App->renderer3D->line_grid_width, 1,30);
		ImGui::Spacing();
	}
	if (ImGui::CollapsingHeader("Vertex Normals")) 
	{
		ImGui::Checkbox("Show Normals", &App->objects->draw_vertex_normals);
		ImGui::ColorEdit3("Normals Color", (float*)&App->objects->vertex_n_color,  ImGuiColorEditFlags_Float);
		ImGui::SliderInt("Vertex Line Length", (int*)&App->objects->vertex_normal_length, 1, 10);
		ImGui::SliderInt("Vertex Line Width", &App->objects->vertex_n_width, 1, 30);
	}
	if (ImGui::CollapsingHeader("Face Normals")) 
	{
		ImGui::Checkbox("Show Face Normals", &App->objects->draw_face_normals);
		ImGui::ColorEdit3("Face Normals Color", (float*)&App->objects->face_n_color, ImGuiColorEditFlags_Float);
		ImGui::SliderInt("Face Line Length", (int*)&App->objects->face_normal_length, 1, 10);
		ImGui::SliderInt("Face Line Width", &App->objects->face_n_width, 1, 30);
	}
	ImGui::End();
}
