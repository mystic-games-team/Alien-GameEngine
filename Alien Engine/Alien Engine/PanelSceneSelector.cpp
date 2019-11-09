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
			App->file_system->Remove(App->objects->current_scene.full_path.data());
			App->objects->SaveScene(App->objects->current_scene.full_path.data());
		}
		break;
	case SceneSelectorState::SAVE_AS_NEW: {
		SaveSceneAsNew();
		// si el nom es untitled mirar si  ja hi ha alguna aixi i posa 2
		break; }
	case SceneSelectorState::CREATE_NEW_SCENE:

		break;
	case SceneSelectorState::LOAD_SCENE:

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
	OPENFILENAME ofn;

	static char filename[MAX_PATH];
	static char curr_dir[MAX_PATH];

	GetCurrentDirectoryA(MAX_PATH, curr_dir);
	std::string dir = std::string(curr_dir + std::string("\\") + std::string("Assets\\Scenes")).data();
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "alienScene\0*.alienScene";  //Add a filter, so you can limite the files you want. In this case you can only selet .json files
	ofn.lpstrFile = filename; //This will recieve the name
	ofn.nMaxFile = MAX_PATH; //max size in characters of the path
	ofn.lpstrTitle = "Save As New .alienScene"; //the title of the dialogue box
	ofn.lpstrInitialDir = dir.data();
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_EXPLORER; //Flags that do flag things :)
	if (GetOpenFileNameA(&ofn)) //the function that opens the file folder
	{	
		SetCurrentDirectoryA(curr_dir);
		App->ui->panel_project->RefreshAllNodes();
	}
	else {
		SetCurrentDirectoryA(curr_dir);
	}
	
}
