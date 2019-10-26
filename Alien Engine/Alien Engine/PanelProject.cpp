#include "PanelProject.h"
#include "ModuleFileSystem.h"

PanelProject::PanelProject(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Panel Project", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
	//colum_width[0] = 100;
	//colum_width[1] = 300;

	assets.is_file = false;
	assets.name = "Assets";
	App->file_system->DiscoverEverythig(assets);

	int i = 0;
}
PanelProject::~PanelProject()
{
}

void PanelProject::PanelLogic()
{

	ImGui::Begin("Project", &enabled, ImGuiWindowFlags_NoCollapse);

	ImGui::Columns(2,"##ProjectColums", true);
	//ImGui::SetColumnWidth(0, colum_width[0]);
	//ImGui::SetColumnWidth(1, colum_width[1]);
	ImGui::GetColumnWidth();
	// Colum 1: Move with the folders
	//colum_width[0] = ImGui::GetWindowWidth();



	// Column 2: Files in x folder
	ImGui::NextColumn();
	//colum_width[1] = ImGui::GetWindowWidth();

	ImGui::Text("dsfs");

	ImGui::End();

}

void PanelProject::PrintDirectoryNodes(FileNode & node)
{
	
}
