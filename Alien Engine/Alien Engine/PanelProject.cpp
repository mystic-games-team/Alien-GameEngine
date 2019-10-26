#include "PanelProject.h"
#include "ModuleResources.h"
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
	current_active_folder = assets;
	App->file_system->DiscoverEverythig(assets);
}
PanelProject::~PanelProject()
{
	DeleteNodes(assets);
}

void PanelProject::PanelLogic()
{

	if (change_folder) {
		current_active_folder = current_active_file;
		current_active_file = nullptr;
		change_folder = false;
	}

	ImGui::Begin("Project", &enabled, ImGuiWindowFlags_NoCollapse);

	ImGui::Columns(2,"##ProjectColums", true);

	static bool first = true;
	if (first) {
		ImGui::SetColumnWidth(0, 160);
		first = false;
	}


	colum_width[0] = ImGui::GetColumnWidth();
	PrintDirectoryNodes(assets);
	
	ImGui::NextColumn();
	colum_width[1] = ImGui::GetColumnWidth();

	if (current_active_folder != nullptr) {
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
			| ImGuiTreeNodeFlags_SpanAvailWidth | node_flags | (current_active_folder == node ? ImGuiTreeNodeFlags_Selected : 0));


		if (ImGui::IsItemClicked()) {
			current_active_folder = node;
			current_active_file = nullptr;
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
	if (ImGui::BeginChild("##ProjectChildName", { 0,20 },false,ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
		ImGui::NewLine();
		ImGui::SameLine(0, -2.F);
		if (ImGui::Button("Go Back", { 0,18 })) {
			if (current_active_folder->parent != nullptr) {
				current_active_folder = current_active_folder->parent;
				current_active_file = nullptr;
			}
		}
		ImGui::SameLine(0, -2.F);
		ImGui::Text("|");
		ImGui::SameLine(0, -29.F);
		ImGui::Text(std::string(current_active_folder->path + current_active_folder->name).data());

		ImGui::Separator();

		ImGui::EndChild();
	}
	
	if (ImGui::BeginChild("##ProjectChild")) {

		ImGui::SetWindowFontScale(0.9F);

		ImGui::Columns(int(colum_width[1] / 78), "##ColumnIcons", false);

		for (uint i = 0; i < current_active_folder->children.size(); ++i) {

			ImVec4 color(0, 0, 0, 0);

			if (current_active_file != nullptr && current_active_file == current_active_folder->children[i])
				color = { 0.07F,0.64F,0.73F,1 };

			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, color);
			ImGui::ImageButton((ImTextureID)current_active_folder->children[i]->icon->id, { 53,70 }, { 0,0 }, { 1,1 }, -1, { 0,0,0,0 }, { 1,1,1,1 });
			ImGui::PopStyleColor();

			// set the file clicked
			if (ImGui::IsItemClicked()) {
				current_active_file = current_active_folder->children[i];
			}

			// go in to a folder
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && current_active_file != nullptr && !current_active_file->is_file) {
				change_folder = true;
			}

			ImGui::NewLine();
			ImGui::SameLine();
			
			if (current_active_folder->children[i]->name.length() > 40) {
				char new_char[5];
				memcpy(new_char, current_active_folder->children[i]->name.data(), 4);
				new_char[4] = '\0';
				std::string name(std::string(new_char) + std::string("..."));
				ImGui::Text(name.data());
			}
			else
				ImGui::Text(current_active_folder->children[i]->name.data());
			ImGui::Spacing();

			ImGui::NextColumn();
		}
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
