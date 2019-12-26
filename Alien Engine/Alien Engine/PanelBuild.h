#pragma once

#include "Panel.h"

class PanelBuild : public Panel {

public:

	PanelBuild(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelBuild();

	void PanelLogic();

	void OnPanelDesactive();

private:

	void GetAllScenes(const std::vector<std::string>& directories, const std::vector<std::string>& files, const std::string& current_folder);
	void SelectFile(const char* text, std::string& to_fill, bool file, std::string& full_path);
	void CreateBuild();
private:

	const char* selected = nullptr;
	std::vector<std::string> scenes;

	std::string readme_name;
	std::string readme_fullpath;
	std::string license_name;
	std::string license_fullpath;
	std::string build_name;
	std::string build_folder_fullpath;
	std::string exe_path;
	std::string folder_location;
	char game_name[MAX_PATH] = "MyAwesomeGame";
	char folder_name[MAX_PATH] = "MyAwesomeFolder";
};
