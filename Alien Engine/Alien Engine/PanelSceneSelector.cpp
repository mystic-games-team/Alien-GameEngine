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
	switch (state) {
	case SceneSelectorState::SAVE_SCENE:
		
		break;
	case SceneSelectorState::SAVE_AS_NEW:

		break;
	case SceneSelectorState::CREATE_NEW_SCENE:

		break;
	case SceneSelectorState::LOAD_SCENE:

		break;
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
		CreateNewScene();
		break;
	case SceneSelectorState::LOAD_SCENE:
		LoadScene();
		break;
	}
}

bool PanelSceneSelector::ExistsScene(const char* scene_name_with_extension)
{
	if (App->file_system->ExistsInFolderRecursive(SCENE_FOLDER, scene_name_with_extension)) {
		return true;
	}
	return false;
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

		App->objects->CreateEmptyScene(filename);

		// last of all, refresh nodes because I have no idea if the user has created folders or moved things in the explorer. Users are bad people creating folders without using the alien engine explorer :(
		App->ui->panel_project->RefreshAllNodes();
	}
	else {
		SetCurrentDirectoryA(curr_dir);
	}
}
