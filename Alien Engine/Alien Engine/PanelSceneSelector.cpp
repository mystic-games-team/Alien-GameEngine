#include "PanelSceneSelector.h"
#include "Color.h"
#include "imgui/imgui.h"
#include "ResourceTexture.h"
#include "imgui/imgui_internal.h"
#include "FileNode.h"
#include "PanelProject.h"

PanelSceneSelector::PanelSceneSelector(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Save", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
	enabled = false;
}

PanelSceneSelector::~PanelSceneSelector()
{
}

void PanelSceneSelector::PanelLogic()
{
	if (menu_save_current) {
		MenuSaveCurrentScene();
	}
}

void PanelSceneSelector::OrganizeSave(const SceneSelectorState& state)
{
	switch (state) {
	case SceneSelectorState::SAVE_SCENE:
		if (App->objects->current_scene.is_untitled) {
			OrganizeSave(SceneSelectorState::SAVE_AS_NEW);
		}
		else {
			App->objects->SaveScene(App->objects->current_scene.full_path.data());
		}
		break;
	case SceneSelectorState::SAVE_AS_NEW: {
		SaveSceneAsNew();
		break; }
	case SceneSelectorState::CREATE_NEW_SCENE:
		if (App->objects->current_scene.is_untitled) {
			CreateNewScene();
		}
		else {
			menu_save_current = true;
			create_new = true;
		}
		break;
	case SceneSelectorState::LOAD_SCENE:
		if (App->objects->current_scene.is_untitled) {
			LoadScene();
		}
		else {
			menu_save_current = true;
			load = true;
		}
		break;
	}
}

void PanelSceneSelector::LoadSceneWithPath(const char* path)
{
	scene_to_load = std::string(path);
	menu_save_current = true;
}

void PanelSceneSelector::SaveSceneAsNew()
{
	OPENFILENAME to_save;

	static char filename[MAX_PATH];

	// get the current game directory
	static char curr_dir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, curr_dir);
	
	// add the local path of the scenes folder
	std::string dir = std::string(curr_dir + std::string("\\") + std::string("Assets\\Scenes")).data();
	
	// fill eveything with 0  in order to avoid problems
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&to_save, sizeof(to_save));

	to_save.lStructSize = sizeof(to_save);
	to_save.hwndOwner = NULL;  
	to_save.lpstrFilter = "alienScene\0*.alienScene";  
	to_save.lpstrFile = filename; 
	to_save.nMaxFile = MAX_PATH;
	to_save.lpstrTitle = "Save As New .alienScene"; 
	to_save.lpstrInitialDir = dir.data();
	to_save.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_EXPLORER; 
	if (GetSaveFileNameA(&to_save)) 
	{	
		SetCurrentDirectoryA(curr_dir);

		std::string extension;
		App->file_system->SplitFilePath(filename, nullptr, nullptr, &extension);
		if (!App->StringCmp("alienScene", extension.data()))
			App->objects->SaveScene(std::string(filename + std::string(".alienScene")).data());
		else
			App->objects->SaveScene(filename);

		// last of all, refresh nodes because I have no idea if the user has created folders or moved things in the explorer. Users are bad people creating folders without using the alien engine explorer :(
		App->ui->panel_project->RefreshAllNodes();
	}
	else { 
		SetCurrentDirectoryA(curr_dir);
	}
	
}

void PanelSceneSelector::LoadScene()
{
	OPENFILENAME to_load;

	static char filename[MAX_PATH];

	// get the current game directory
	static char curr_dir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, curr_dir);

	// add the local path of the scenes folder
	std::string dir = std::string(curr_dir + std::string("\\") + std::string("Assets\\Scenes")).data();

	// fill eveything with 0  in order to avoid problems
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&to_load, sizeof(to_load));

	to_load.lStructSize = sizeof(to_load);
	to_load.hwndOwner = NULL;
	to_load.lpstrFilter = "alienScene\0*.alienScene";
	to_load.lpstrFile = filename;
	to_load.nMaxFile = MAX_PATH;
	to_load.lpstrTitle = "Load a .alienScene";
	to_load.lpstrInitialDir = dir.data();
	to_load.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	if (GetOpenFileNameA(&to_load))
	{
		SetCurrentDirectoryA(curr_dir);

		App->objects->LoadScene(filename);

		// last of all, refresh nodes because I have no idea if the user has created folders or moved things in the explorer. Users are bad people creating folders without using the alien engine explorer :(
		App->ui->panel_project->RefreshAllNodes();
	}
	else {
		SetCurrentDirectoryA(curr_dir);
	}
}

void PanelSceneSelector::CreateNewScene()
{
	OPENFILENAME to_save;

	static char filename[MAX_PATH];

	// get the current game directory
	static char curr_dir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, curr_dir);

	// add the local path of the scenes folder
	std::string dir = std::string(curr_dir + std::string("\\") + std::string("Assets\\Scenes")).data();

	// fill eveything with 0  in order to avoid problems
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&to_save, sizeof(to_save));

	to_save.lStructSize = sizeof(to_save);
	to_save.hwndOwner = NULL;
	to_save.lpstrFilter = "alienScene\0*.alienScene";
	to_save.lpstrFile = filename;
	to_save.nMaxFile = MAX_PATH;
	to_save.lpstrTitle = "Create a new .alienScene";
	to_save.lpstrInitialDir = dir.data();
	to_save.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	if (GetSaveFileNameA(&to_save))
	{
		SetCurrentDirectoryA(curr_dir);

		std::string extension;
		App->file_system->SplitFilePath(filename, nullptr, nullptr, &extension);

		if (!App->StringCmp("alienScene",extension.data()))
			App->objects->CreateEmptyScene(std::string(filename + std::string(".alienScene")).data());
		else 
			App->objects->CreateEmptyScene(filename);

		// last of all, refresh nodes because I have no idea if the user has created folders or moved things in the explorer. Users are bad people creating folders without using the alien engine explorer :(
		App->ui->panel_project->RefreshAllNodes();
	}
	else {
		SetCurrentDirectoryA(curr_dir);
	}
}

void PanelSceneSelector::MenuSaveCurrentScene()
{
	ImGui::OpenPopup("Scene might have been modified");
	ImGui::SetNextWindowSize({ 240,140 });
	if (ImGui::BeginPopupModal("Scene might have been modified", &menu_save_current, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Spacing();

		ImGui::Text("Do you want to save changes in:");

		ImGui::Spacing();

		ImGui::Text(App->file_system->GetBaseFileNameWithExtension(App->objects->current_scene.full_path.data()).data());

		ImGui::Spacing();

		ImGui::Text("Your changes will be lost if you\ndon't save them.");

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Save", { 65,20 })) {
			if (App->objects->current_scene.is_untitled) {
				SaveSceneAsNew();
			}
			else {
				App->objects->SaveScene(App->objects->current_scene.full_path.data());
			}
			menu_save_current = false;
		}
		

		ImGui::SameLine();

		if (ImGui::Button("Don't save", { 80,20 })) {
			menu_save_current = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", { 65,20 })) {
			scene_to_load.clear();
			menu_save_current = false;
			load = false;
			create_new = false;
		}

		ImGui::EndPopup();
	}
	else {
		load = false;
		create_new = false;
		scene_to_load.clear();
	}

	if (!menu_save_current) {
		if (load) {
			load = false;
			LoadScene();
		}
		else if (create_new) {
			create_new = false;
			CreateNewScene();
		}
		else if (!scene_to_load.empty()) {
			App->objects->LoadScene(scene_to_load.data());
			scene_to_load.clear();
		}
	}
}
