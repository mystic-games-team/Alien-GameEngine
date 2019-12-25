#include "PanelBuild.h"

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
		if (ImGui::Button("Add README", { 90,0 })) {

		}
		ImGui::SameLine();

		if (readme_path.empty()) {
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.7f, 0.2F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.7f, 0.2F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.7f, 0.2F, 0.2f, 1 });
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.2f, 0.5F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.7f, 0.2F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.7f, 0.2F, 0.2f, 1 });
		}
		ImGui::Button((readme_path.empty()) ? "NO README" : readme_path.data(), {ImGui::GetWindowWidth() * 0.61F, 0});
		ImGui::PopStyleColor(3);

		ImGui::Spacing();

		ImGui::SetCursorPosX(10);
		if (ImGui::Button("Add LICENSE", { 90,0 })) {

		}
		ImGui::SameLine();

		if (license_path.empty()) {
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.7f, 0.2F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.7f, 0.2F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.7f, 0.2F, 0.2f, 1 });
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.2f, 0.5F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.7f, 0.2F, 0.2f, 1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.7f, 0.2F, 0.2f, 1 });
		}
		ImGui::Button((license_path.empty()) ? "NO LICENSE" : license_path.data(), { ImGui::GetWindowWidth() * 0.61F, 0 });
		ImGui::PopStyleColor(3);

		ImGui::EndPopup();
	}
}

void PanelBuild::OnPanelDesactive()
{
	selected = nullptr;
	scenes.clear();
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
