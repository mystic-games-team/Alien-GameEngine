#include "PanelProject.h"
#include "ModuleResources.h"
#include "ModuleFileSystem.h"
#include "ResourceTexture.h"

PanelProject::PanelProject(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Panel Project", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);

	assets = App->resources->assets;
	current_active_folder = assets;

	go_back_folder.name = "Go Back";
	go_back_folder.is_file = false;
	go_back_folder.icon = App->resources->icons.folder;


	//std::rename("Assets/Models/BakerHouse.fbx", "Assets/BakerHouse.fbx");
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
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				current_active_folder = current_active_folder->parent;
				ImGui::EndChild();
				return;
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

			// right click in file/folder
			if (current_active_file != nullptr && current_active_file == current_active_folder->children[i] && ImGui::BeginPopupContextItem()) {
				pop_up_item = true;

				if (!current_active_file->is_base_file) { // so can not be modified
					if (ImGui::MenuItem("Delete")) {
						// TODO: delete
						// Delete selected asset? You can not undo this action
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

			// drag
			if (current_active_file != nullptr && current_active_file == current_active_folder->children[i] && !current_active_file->is_base_file) {
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoDisableHover)) {
					std::string drag_id;

					switch (current_active_file->type) {
					case FileDropType::MODEL3D:
						drag_id = DROP_ID_MODEL;
						break;
					case FileDropType::TEXTURE:
						drag_id = DROP_ID_TEXTURE;
						break;
					case FileDropType::FOLDER:
						drag_id = DROP_ID_FOLDER;
						break;
					}

					ImGui::SetDragDropPayload(drag_id.data(), &current_active_file, sizeof(FileNode), ImGuiCond_Once);
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
							// TODO: if file name is changed, set this new name to meta data need change!!!!!
							switch (current_active_folder->children[i]->type) {
							case FileDropType::MODEL3D: {
								std::string meta_path = LIBRARY_MODELS_FOLDER + App->file_system->GetCurrentFolder(current_active_folder->children[i]->path) + App->file_system->GetBaseFileName(current_active_folder->children[i]->name.data()) + ".alien";
								App->resources->SetNewMetaName(name, meta_path, current_active_folder->children[i]->type);
								break; }
							case FileDropType::TEXTURE:
								// TODO: 
								break;
							}
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

			ImGui::Spacing();

			ImGui::NextColumn();
		}
		

		// right click in window
		if (!pop_up_item && ImGui::BeginPopupContextWindow()) {
			if (ImGui::MenuItem("Create New Folder")) {
				int folder_number = 0;
				std::string folder_name = "NewFolder" + std::to_string(folder_number);

				for (uint i = 0; i < current_active_folder->children.size(); ++i) {
					if (App->StringCmp(current_active_folder->children[i]->name.data(), folder_name.data())) {
						++folder_number;
						folder_name = "NewFolder" + std::to_string(folder_number);
						i = 0;
					}
				}
				FileNode* folder = new FileNode(folder_name.data(), false, current_active_folder);
				folder->changing_name = true;
				current_active_folder->children.push_back(folder);
				App->file_system->CreateDirectoryA(std::string(folder->path + folder->name).data());
			}
			if (ImGui::MenuItem("Show In Explorer")) {
				// TODO: open explorer
				char name[500];
				GetCurrentDirectoryA(500, name);
				std::string current(name + std::string("/") + current_active_folder->path);
				ShellExecute(NULL, "open", current.data(), NULL, NULL, SW_SHOWDEFAULT);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Refresh")) {
				// TODO: refresh

			}
			ImGui::EndPopup();
		}
	}
	ImGui::EndChild();

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
