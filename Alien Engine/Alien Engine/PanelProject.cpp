#include "PanelProject.h"
#include "ModuleResources.h"
#include "ModuleFileSystem.h"
#include "ResourceTexture.h"
#include <filesystem>
#include "imgui/imgui_internal.h"
#include "ResourceModel.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

PanelProject::PanelProject(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Panel Project", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);

	assets = App->resources->assets;
	current_active_folder = assets;
	
	go_back_folder.name = "Go Back";
	go_back_folder.is_file = false;
	go_back_folder.icon = App->resources->icons.return_icon;
	go_back_folder.path.clear();
	

	//std::rename("Assets/Models/BakerHouse.fbx", "Assets/BakerHouse.fbx");
}
PanelProject::~PanelProject()
{
	delete assets;
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
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;
		
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
		ImGui::Text(current_active_folder->path.data());
		ImGui::Separator();
	}
	ImGui::EndChild();
	if (ImGui::BeginChild("##ProjectChild")) {

		bool pop_up_item = false;

		ImGui::SetWindowFontScale(0.9F);

		ImGui::Columns(int(colum_width[1] / 78), "##ColumnIcons", false);

		ImVec4 color(0, 0, 0, 0);

		// put a folder to go back
		if (current_active_folder != assets) {

			if (current_active_file == &go_back_folder) {
				color = { 0.07F,0.64F,0.73F,1 };
			}

			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, color);
			ImGui::ImageButton((ImTextureID)go_back_folder.icon->id, { 53,70 }, { 0,0 }, { 1,1 }, -1, { 0,0,0,0 }, { 1,1,1,1 });
			ImGui::PopStyleColor();

			if (ImGui::IsItemClicked()) {
				current_active_file = &go_back_folder;
			}
			
			// go back a folder
			if (ImGui::IsItemClicked()) {
				current_active_folder = current_active_folder->parent;
				ImGui::EndChild();
				return;
			}

			if (ImGui::IsItemHovered()) {
				MoveToFolder(&go_back_folder, false);
			}

			ImGui::NewLine();
			ImGui::SameLine();

			ImGui::Text(go_back_folder.name.data());

			ImGui::NextColumn();
		}


		for (uint i = 0; i < current_active_folder->children.size(); ++i) {
			color = { 0, 0, 0, 0 };
			if (current_active_file != nullptr && current_active_file == current_active_folder->children[i])
				color = { 0.07F,0.64F,0.73F,1 };

			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, color);
			ImGui::ImageButton((ImTextureID)current_active_folder->children[i]->icon->id, { 53,70 }, { 0,0 }, { 1,1 }, -1, { 0,0,0,0 }, { 1,1,1,1 });
			ImGui::PopStyleColor();

			// set the file clicked
			if (ImGui::IsItemClicked() || (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && ImGui::IsMouseClicked(1))) {
				current_active_file = current_active_folder->children[i];
			}

			if (ImGui::IsItemHovered() && current_active_file != current_active_folder->children[i]) {
				if (MoveToFolder(current_active_folder->children[i], true))
					break;
			}

			// right click in file/folder
			RightClickInFileOrFolder(i, pop_up_item);

			// drag
			if (current_active_file != nullptr && current_active_file == current_active_folder->children[i] && !current_active_file->is_base_file) {
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoDisableHover)) {
					ImGui::SetDragDropPayload(DROP_ID_PROJECT_NODE, &current_active_file, sizeof(FileNode), ImGuiCond_Once);
					ImGui::SetCursorPosX(((ImGui::GetWindowWidth())*0.5f)-26);
					ImGui::Image((ImTextureID)current_active_file->icon->id, { 53,70 });
					ImGui::Text(current_active_file->name.data());

					ImGui::EndDragDropSource();
				}
				
			}
			
			// go into a folder
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && current_active_file != nullptr && !current_active_file->is_file) {
				change_folder = true;
			}

			ImGui::NewLine();
			ImGui::SameLine();
			
			PrintNodeNameUnderIcon(i);

			ImGui::Spacing();

			ImGui::NextColumn();
		}

		// right click in window
		RightClickToWindow(pop_up_item);
	}
	ImGui::EndChild();

	DeleteSelectedAssetPopUp();
}

void PanelProject::DeleteSelectedAssetPopUp()
{
	if (to_delete_menu && current_active_file != nullptr) {
		ImGui::OpenPopup("Delete Selected Asset?");
		ImGui::SetNextWindowSize({ 410,100 });
		if (ImGui::BeginPopupModal("Delete Selected Asset?", &to_delete_menu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			std::string path;
			if (current_active_file->is_file)
				path = std::string(current_active_file->path + current_active_file->name).data();
			else
				path = current_active_file->path.data();

			ImGui::Text(path.data());

			ImGui::Text("You cannot undo this action.");

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::NewLine();
			ImGui::SameLine(150);
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.65F,0,0,1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.8F,0,0,1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.95F,0,0,1 });
			if (ImGui::Button("Delete")) {
				std::vector<FileNode*>::iterator item = current_active_folder->children.begin();

				// set nullptr to this resource of the gameobjects that are using it
				current_active_file->RemoveResourceOfGameObjects();

				// this removes all meta data of the nodes
				current_active_file->DeleteNodeData();
				
				for (; item != current_active_folder->children.end(); ++item) {
					if (*item != nullptr && *item == current_active_file) {
						delete* item;
						*item = nullptr;
						current_active_folder->children.erase(item);
						break;
					}
				}
				current_active_file = nullptr;
				to_delete_menu = false;
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				to_delete_menu = false;
			}

			ImGui::EndPopup();
		}
	}
}

void PanelProject::RightClickInFileOrFolder(const uint& i, bool& pop_up_item)
{
	if (current_active_file != nullptr && current_active_file == current_active_folder->children[i] && ImGui::BeginPopupContextItem()) {
		pop_up_item = true;

		if (!current_active_file->is_base_file) { // so can not be modified
			if (ImGui::MenuItem("Delete")) {
				to_delete_menu = true;
			}
			if (ImGui::MenuItem("Rename")) {
				current_active_folder->children[i]->changing_name = true;
			}
		}
		if (ImGui::MenuItem("Copy Path")) {
			// TODO: copy path
		}
		ImGui::EndPopup();
	}
}

void PanelProject::PrintNodeNameUnderIcon(const uint& i)
{
	if (current_active_folder->children[i]->changing_name) {
		char name[100];

		if (current_active_folder->children[i]->is_file)
			strcpy_s(name, 100, App->file_system->GetBaseFileName(current_active_folder->children[i]->name.data()).data());
		else
			strcpy_s(name, 100, current_active_folder->children[i]->name.data());

		if (ImGui::InputText("##nodechangename", name, 100, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
			current_active_folder->children[i]->changing_name = false;

			std::string name_before_rename = name;

			if (current_active_folder->children[i]->is_file) {
				std::string extension;
				App->file_system->SplitFilePath(current_active_folder->children[i]->name.data(), nullptr, nullptr, &extension);
				name_before_rename += std::string(".") + extension;
			}

			if (rename(std::string(current_active_folder->path + std::string("/") + current_active_folder->children[i]->name).data(), std::string(current_active_folder->path + std::string("/") + name_before_rename).data()) == 0) {
				if (current_active_folder->children[i]->is_file) {
					std::string current_meta_path = App->file_system->GetPathWithoutExtension(current_active_folder->children[i]->path + current_active_folder->children[i]->name) + "_meta.alien";
					std::string next_meta_name = App->file_system->GetPathWithoutExtension(current_active_folder->children[i]->path + name_before_rename) + "_meta.alien";
					rename(current_meta_path.data(), next_meta_name.data());
				}
				else {
					current_active_folder->children[i]->path = current_active_folder->children[i]->parent->path + name_before_rename + "/";
				}
				current_active_folder->children[i]->name = name_before_rename;

				LOG("New file/folder renamed correctly to %s", current_active_folder->children[i]->name.data());
			}
			else {
				LOG("Failing while renaming %s to %s because this name already exists", current_active_folder->children[i]->name.data(), name);
			}
		}

		if (!ImGui::IsItemClicked() && ImGui::IsMouseClicked(0)) {
			current_active_folder->children[i]->changing_name = false;
		}
	}
	else {	// make the name smaller
		if (current_active_folder->children[i]->name.length() > 7) {
			char new_char[8];
			memcpy(new_char, current_active_folder->children[i]->name.data(), 7);
			new_char[7] = '\0';
			std::string name(std::string(new_char) + std::string("..."));
			ImGui::Text(name.data());
		}
		else
			ImGui::Text(current_active_folder->children[i]->name.data());
	}
}

void PanelProject::RightClickToWindow(bool pop_up_item)
{
	if (!pop_up_item && ImGui::BeginPopupContextWindow()) {
		if (ImGui::MenuItem("Create New Folder")) {
			int folder_number = 0;
			std::string folder_name = "NewFolder" + std::to_string(folder_number);

			for (uint i = 0; i < current_active_folder->children.size(); ++i) {
				if (App->StringCmp(current_active_folder->children[i]->name.data(), folder_name.data())) {
					++folder_number;
					folder_name = "NewFolder" + std::to_string(folder_number);
					i = -1;
				}
			}
			FileNode* folder = new FileNode(std::string(current_active_folder->path + folder_name + std::string("/")), folder_name.data(), false, current_active_folder);
			folder->changing_name = true;
			current_active_folder->children.push_back(folder);
			App->file_system->CreateDirectoryA(std::string(folder->path).data());
		}
		if (ImGui::MenuItem("Show In Explorer")) {
			char name[500];
			GetCurrentDirectoryA(500, name);
			std::string current(name + std::string("/") + current_active_folder->path);
			ShellExecute(NULL, "open", current.data(), NULL, NULL, SW_SHOWDEFAULT);
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Refresh")) {
			RefreshAllNodes();
		}
		ImGui::EndPopup();
	}
}

bool PanelProject::MoveToFolder(FileNode* node, bool inside)
{
	bool ret = false;
	if (node->is_file || ImGui::IsMouseDragging())
		return ret;

	if (ImGui::BeginDragDropTargetCustom(ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()), ImGui::GetID("##ProjectChild"))) {
		const ImGuiPayload* payload = ImGui::GetDragDropPayload();
		if (payload != nullptr && payload->IsDataType(DROP_ID_PROJECT_NODE)) {
			ret = true;
			FileNode* node_to_move = *(FileNode**)payload->Data;

			if (inside) {
				if (node_to_move->is_file) { // move file up
					if (rename(std::string(node_to_move->path + node_to_move->name).data(), std::string(node->path + node_to_move->name).data()) == 0) {
						
						// move the .alien too
						rename(std::string(App->file_system->GetPathWithoutExtension(std::string(node_to_move->path + node_to_move->name)) + "_meta.alien").data(), std::string(App->file_system->GetPathWithoutExtension(std::string(node->path + node_to_move->name)) + "_meta.alien").data());

						FileNode* parent = node_to_move->parent;
						std::vector<FileNode*>::iterator item = parent->children.begin();
						for (; item != parent->children.end(); ++item) {
							if (*item != nullptr && *item == node_to_move) {
								delete* item;
								*item = nullptr;
								parent->children.erase(item);
								break;
							}
						}
						node->DeleteChildren();
						App->file_system->DiscoverEverythig(node);
					}
					else {
						LOG("Fail when moving %s to %s", std::string(node_to_move->path + node_to_move->name).data(), std::string(node->path + node_to_move->name).data());
					}
				}
				else { // move folder down
					std::experimental::filesystem::rename(node_to_move->path.data(), std::string(node->path + node_to_move->name + std::string("/")).data());
					if (!std::experimental::filesystem::exists(node_to_move->path.data())) {
						FileNode* parent = node_to_move->parent;
						std::vector<FileNode*>::iterator item = parent->children.begin();
						for (; item != parent->children.end(); ++item) {
							if (*item != nullptr && *item == node_to_move) {
								delete* item;
								*item = nullptr;
								parent->children.erase(item);
								break;
							}
						}
						node->DeleteChildren();
						App->file_system->DiscoverEverythig(node);
					}
					else {
						LOG("Could not move %s to %s", node_to_move->path.data(), std::string(node->path + node_to_move->name + std::string("/")).data());
					}
				}
			}
			else {
				if (node_to_move->is_file) { // move file up
					if (rename(std::string(node_to_move->path + node_to_move->name).data(), std::string(node_to_move->parent->parent->path + node_to_move->name).data()) == 0) {
						std::string actual_folder_path = current_active_folder->path;

						// move the .alien too
						rename(std::string(App->file_system->GetPathWithoutExtension(std::string(node_to_move->path + node_to_move->name)) + "_meta.alien").data(), std::string(App->file_system->GetPathWithoutExtension(std::string(node_to_move->parent->parent->path + node_to_move->name)) + "_meta.alien").data());

						FileNode* next_parent = node_to_move->parent->parent;

						next_parent->DeleteChildren();
						App->file_system->DiscoverEverythig(next_parent);
						current_active_folder = next_parent->FindChildrenByPath(actual_folder_path);
						current_active_file = nullptr;
					}
					else {
						LOG("Fail when moving %s to %s", std::string(node_to_move->path + node_to_move->name).data(), std::string(node->path + node_to_move->name).data());
					}
				}
				else { // move folder up
					std::experimental::filesystem::rename(node_to_move->path.data(), std::string(node_to_move->parent->parent->path + node_to_move->name + std::string("/")).data());
					if (!std::experimental::filesystem::exists(node_to_move->path.data())) {
						FileNode* next_parent = node_to_move->parent->parent;
						std::string actual_folder_path = current_active_folder->path;

						next_parent->DeleteChildren();
						App->file_system->DiscoverEverythig(next_parent);
						current_active_folder = next_parent->FindChildrenByPath(actual_folder_path);
						current_active_file = nullptr;
					}
					else {
						LOG("Could not move %s to %s", node_to_move->path.data(), std::string(node_to_move->parent->parent->path + node_to_move->name + std::string("/")).data());
					}
				}
			}

			ImGui::ClearDragDrop();
		}
		ImGui::EndDragDropTarget();
	}
	return ret;
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

void PanelProject::RefreshAllNodes()
{
	std::string current_folder_path = current_active_folder->path;
	assets->DeleteChildren();
	App->file_system->DiscoverEverythig(assets);
	current_active_folder = assets->FindChildrenByPath(current_folder_path);
	if (current_active_folder == nullptr || (current_active_folder != nullptr && current_active_folder->is_file))
		current_active_folder = assets;
	current_active_file = nullptr;
}

