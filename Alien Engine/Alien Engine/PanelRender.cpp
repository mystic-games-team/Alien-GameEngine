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
	if (ImGui::IsWindowHovered())
		App->camera->is_scene_hovered = false;
	if (ImGui::CollapsingHeader("Mesh Settings")) 
	{
		ImGui::Spacing();
		if (ImGui::Checkbox(" View Mesh", &App->objects->view_mesh_mode)) {
			App->objects->view_mesh_mode = !App->objects->view_mesh_mode;
			App->objects->ChangeViewMeshMode();
		}
		ImGui::SameLine();
		if (ImGui::Checkbox(" Wireframe Mode", &App->objects->wireframe_mode)) {
			App->objects->wireframe_mode = !App->objects->wireframe_mode;
			App->objects->ChangeWireframeMode();
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50);
		ImGui::SliderInt("Mesh Line Width", &App->objects->mesh_line_width, 1, 30);
		ImGui::Spacing();
		ImGui::ColorEdit3("Mesh Color", (float*)& App->objects->mesh_color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float);
	}
	if (ImGui::CollapsingHeader("Bounding Box Options"))
	{
		ImGui::Spacing();
		if (ImGui::Checkbox("View AABB", &App->objects->draw_all_AABB))
		{
			App->objects->draw_all_AABB = !App->objects->draw_all_AABB;
			App->objects->ChangeEnableAABB();
		}
		ImGui::ColorEdit3("AABB Color", &App->objects->global_AABB_color, ImGuiColorEditFlags_Float);
		ImGui::SliderInt("AABB Line Width", (int*)&App->objects->AABB_line_width, 1, 30);

		ImGui::Spacing();
		if (ImGui::Checkbox("View OBB", &App->objects->draw_all_OBB))
		{
			App->objects->draw_all_OBB = !App->objects->draw_all_OBB;
			App->objects->ChangeEnableOBB();
		}
		ImGui::ColorEdit3("OBB Color", &App->objects->global_OBB_color, ImGuiColorEditFlags_Float);
		ImGui::SliderInt("OBB Line Width", (int*)&App->objects->OBB_line_width, 1, 30);
	}
	if (ImGui::CollapsingHeader("Background")) 
	{
		ImGui::Spacing();
		ImGui::ColorEdit3("Background Color", (float*)&App->renderer3D->background_color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float);
		ImGui::Spacing();
	}
	if (ImGui::CollapsingHeader("Render Z-Buffer")) {
		ImGui::Spacing();
		if (ImGui::Checkbox("View Z-Buffer", &App->renderer3D->render_zbuffer)) {
			App->renderer3D->ChangeDrawFrameBuffer(App->renderer3D->render_zbuffer);
		}
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
		if (ImGui::Checkbox("Show Vertex Normals", &App->objects->draw_vertex_normals)) {
			App->objects->draw_vertex_normals = !App->objects->draw_vertex_normals;
			App->objects->ChangeEnableNormalVertex();
		}
		ImGui::ColorEdit3("Normals Color", (float*)&App->objects->vertex_n_color,  ImGuiColorEditFlags_Float);
		ImGui::SliderInt("Vertex Line Length", (int*)&App->objects->vertex_normal_length, 1, 10);
		ImGui::SliderInt("Vertex Line Width", &App->objects->vertex_n_width, 1, 30);
	}
	if (ImGui::CollapsingHeader("Face Normals")) 
	{
		if (ImGui::Checkbox("Show Face Normals", &App->objects->draw_face_normals)) {
			App->objects->draw_face_normals = !App->objects->draw_face_normals;
			App->objects->ChangeEnableNormalFace();
		}
		ImGui::ColorEdit3("Face Normals Color", (float*)&App->objects->face_n_color, ImGuiColorEditFlags_Float);
		ImGui::SliderInt("Face Line Length", (int*)&App->objects->face_normal_length, 1, 10);
		ImGui::SliderInt("Face Line Width", &App->objects->face_n_width, 1, 30);
	}
	if (ImGui::CollapsingHeader("Outline")) {
		ImGui::Checkbox("Active Outline", &App->objects->outline);
		ImGui::ColorEdit3("Parent Color", (float*)& App->objects->parent_outline_color, ImGuiColorEditFlags_Float);
		ImGui::SliderInt("Parent Line Width", (int*)&App->objects->parent_line_width, 1, 30);
		ImGui::ColorEdit3("No children Color", (float*)& App->objects->no_child_outline_color, ImGuiColorEditFlags_Float);
		ImGui::SliderInt("No children Line Width", (int*)&App->objects->no_child_line_width, 1, 30);
	}
	ImGui::End();
}
