#include "PanelBuild.h"
#include <shlobj.h>

PanelBuild::PanelBuild(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Build", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
}


PanelBuild::~PanelBuild()
{
}

void PanelBuild::PanelLogic()
{
	if (scenes.empty()) {
		std::vector<std::string> files;
		std::vector<std::string> directories;
		App->file_system->DiscoverFiles(SCENE_FOLDER, files, directories);
		GetAllScenes(directories, files, SCENE_FOLDER);
	}

	ImGui::OpenPopup("Build Settings");
	ImGui::SetNextWindowSize({ 300,400 });
	if (ImGui::BeginPopupModal("Build Settings", &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Text("Select the first scene for the build");
		ImGui::Spacing();
		ImGui::BeginChild("#BuildChildSceneSelector", { 0, 175 }, true);

		for (uint i = 0; i < scenes.size(); ++i) {
			bool exists = selected == scenes[i].data();
			ImGui::Selectable(std::string("##" + scenes[i]).data(), exists, ImGuiSelectableFlags_AllowItemOverlap);
			if (ImGui::IsItemClicked()) {
				if (exists) {
					selected = nullptr;
				}
				else {
					selected = scenes[i].data();
				}
			}
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5F - ImGui::CalcTextSize(scenes[i].data()).x * 0.5F);
			ImGui::Text(scenes[i].data());
		}
		ImGui::EndChild();
		ImGui::Spacing();

		ImGui::SetCursorPosX(10);
		ImGui::InputText("Game Title", game_name, MAX_PATH, ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::Spacing();

		ImGui::SetCursorPosX(10);
		ImGui::InputText("Folder Name", folder_name, MAX_PATH, ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::Spacing();

		ImGui::SetCursorPosX(10);
		if (ImGui::Button("Add README", { 90,0 })) {
			SelectFile("Select the README file", readme_name, true, readme_fullpath);
		}
		ImGui::SameLine();

		if (readme_name.empty()) {
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.7f, 0.2F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.7f, 0.2F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.7f, 0.2F, 0.2f, 1 });
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.2f, 0.5F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.2f, 0.5F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.2f, 0.5F, 0.2f, 1 });
		}
		ImGui::Button((readme_name.empty()) ? "NO README" : readme_name.data(), {ImGui::GetWindowWidth() * 0.61F, 0});
		ImGui::PopStyleColor(3);

		ImGui::Spacing();

		ImGui::SetCursorPosX(10);
		if (ImGui::Button("Add LICENSE", { 90,0 })) {
			SelectFile("Select the LICENSE file", license_name, true, license_fullpath);
		}
		ImGui::SameLine();

		if (license_name.empty()) {
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.7f, 0.2F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.7f, 0.2F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.7f, 0.2F, 0.2f, 1 });
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.2f, 0.5F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.2f, 0.5F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.2f, 0.5F, 0.2f, 1 });
		}
		ImGui::Button((license_name.empty()) ? "NO LICENSE" : license_name.data(), { ImGui::GetWindowWidth() * 0.61F, 0 });
		ImGui::PopStyleColor(3);
		ImGui::Spacing();

		ImGui::SetCursorPosX(10);
		if (ImGui::Button("Build Folder", { 90,0 })) {
			SelectFile("Select the build folder", build_name, false, build_folder_fullpath);
		}
		ImGui::SameLine();

		if (build_name.empty()) {
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.7f, 0.2F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.7f, 0.2F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.7f, 0.2F, 0.2f, 1 });
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.2f, 0.5F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.2f, 0.5F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.2f, 0.5F, 0.2f, 1 });
		}
		ImGui::Button((build_name.empty()) ? "NO BUILD FOLDER" : build_name.data(), { ImGui::GetWindowWidth() * 0.61F, 0 });
		ImGui::PopStyleColor(3);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::SetCursorPosX(45);
		ImGui::Button("Build", { 100,0 });
		ImGui::SameLine();
		ImGui::Button("Build and Run", { 100,0 });

		ImGui::EndPopup();
	}
	else {
		OnPanelDesactive();
	}
}

void PanelBuild::OnPanelDesactive()
{
	selected = nullptr;
	scenes.clear();
	readme_name.clear();
	readme_fullpath.clear();
	license_name.clear();
	license_fullpath.clear();
	build_name.clear();
	build_folder_fullpath.clear();
	strcpy(game_name, "MyAwesomeGame");
	strcpy(folder_name, "FolderName");
}

void PanelBuild::GetAllScenes(const std::vector<std::string>& directories, const std::vector<std::string>& files, const std::string& current_folder)
{
	for (uint i = 0; i < files.size(); ++i) {
		scenes.push_back(App->file_system->GetBaseFileName(files[i].data()));
	}
	if (!directories.empty()) {
		std::vector<std::string> new_files;
		std::vector<std::string> new_directories;

		for (uint i = 0; i < directories.size(); ++i) {
			std::string dir = current_folder + directories[i] + "/";
			App->file_system->DiscoverFiles(dir.data(), new_files, new_directories);
			GetAllScenes(new_directories, new_files, dir);
		}
	}
}

void PanelBuild::SelectFile(const char* text, std::string& to_fill, bool file, std::string& full_path)
{
	if (file) {
		OPENFILENAME to_load;

		static char filename[MAX_PATH];

		// get the current game directory
		static char curr_dir[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, curr_dir);

		std::string dir = std::string(curr_dir + std::string("\\") + std::string("Assets")).data();

		// fill eveything with 0  in order to avoid problems
		ZeroMemory(&filename, sizeof(filename));
		ZeroMemory(&to_load, sizeof(to_load));

		to_load.lStructSize = sizeof(to_load);
		to_load.hwndOwner = NULL;
		to_load.lpstrFilter = NULL;
		to_load.lpstrFile = filename;
		to_load.nMaxFile = MAX_PATH;
		to_load.lpstrTitle = text;
		to_load.lpstrInitialDir = dir.data();
		to_load.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_EXPLORER;
		if (GetOpenFileNameA(&to_load))
		{
			SetCurrentDirectoryA(curr_dir);
			full_path = filename;
			App->file_system->NormalizePath(full_path);
			to_fill = App->file_system->GetBaseFileName(full_path.data());
		}
		else {
			SetCurrentDirectoryA(curr_dir);
		}
	}
	else {
		BROWSEINFO to_load;
		char szPath[MAX_PATH + 1];
		LPITEMIDLIST pidl;
		BOOL bResult = FALSE;

		LPMALLOC pMalloc;

		if (SUCCEEDED(SHGetMalloc(&pMalloc)))
		{
			static char filename[MAX_PATH];

			// get the current game directory
			static char curr_dir[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, curr_dir);

			std::string dir = std::string(curr_dir + std::string("\\") + std::string("Assets")).data();

			// fill eveything with 0  in order to avoid problems
			ZeroMemory(&filename, sizeof(filename));
			ZeroMemory(&to_load, sizeof(to_load));

			to_load.hwndOwner = NULL;
			to_load.pszDisplayName = filename;
			to_load.lpszTitle = text;

			pidl = SHBrowseForFolder(&to_load);
			if (pidl)
			{
				if (SHGetPathFromIDList(pidl, szPath))
				{
					bResult = TRUE;
					strcpy(filename, szPath);
					full_path = filename;
					App->file_system->NormalizePath(full_path);
					to_fill = App->file_system->GetBaseFileName(full_path.data());
				}

				pMalloc->Free(pidl);
				pMalloc->Release();
				SetCurrentDirectoryA(curr_dir);
			}
			else {
				SetCurrentDirectoryA(curr_dir);
			}
		}
	}
}
