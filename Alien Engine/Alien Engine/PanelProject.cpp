#include "PanelProject.h"
#include "ModuleFileSystem.h"

PanelProject::PanelProject(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Panel Project", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
	//colum_width[0] = 100;
	//colum_width[1] = 300;

	assets = new FileNode();
	assets->is_file = false;
	assets->name = "Assets";
	current_node = assets;
	App->file_system->DiscoverEverythig(assets);
}
PanelProject::~PanelProject()
{
	DeleteNodes(assets);
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

	PrintDirectoryNodes(assets);

	// Column 2: Files in x folder
	ImGui::NextColumn();
	//colum_width[1] = ImGui::GetWindowWidth();

	if (current_node != nullptr) {
		SeeFiles();
	}

	ImGui::End();

}

void PanelProject::PrintDirectoryNodes(FileNode * node)
{
	if (!node->is_file) {
		ImGuiTreeNodeFlags_ node_flags = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;

		// look if is lead or not
		for (uint i = 0; i < node->children.size(); ++i) {
			if (node->children[i] != nullptr && !node->children[i]->is_file) {
				node_flags = ImGuiTreeNodeFlags_None;
				break;
			}
		}

		bool is_open = ImGui::TreeNodeEx(node->name.data(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
			| ImGuiTreeNodeFlags_SpanAvailWidth | node_flags | (current_node == node ? ImGuiTreeNodeFlags_Selected : 0));


		if (ImGui::IsItemClicked()) {
			current_node = node;
		}

		if (is_open) {
			for (uint i = 0; i < node->children.size(); ++i) {
				if (node->children[i] != nullptr && !node->children[i]->is_file) {
					PrintDirectoryNodes(node->children[i]);
				}
			}
			ImGui::TreePop();
		}
	}
}

void PanelProject::SeeFiles()
{
	if (ImGui::BeginChild("##ProjectChild")) {

		ImGui::Text(std::string(current_node->path + current_node->name).data());
		ImGui::Separator();

		ImGui::EndChild();
	}


}

void PanelProject::DeleteNodes(FileNode* node)
{
	if (!node->children.empty()) {
		for (uint i = 0; i < node->children.size(); ++i) {
			if (node->children[i] != nullptr)
				DeleteNodes(node->children[i]);
		}
	}
	delete node;
}
