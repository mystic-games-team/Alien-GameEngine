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
private:

	const char* selected = nullptr;
	std::vector<std::string> scenes;

	std::string readme_path;
	std::string license_path;
	std::string build_folder;
	char game_name[MAX_PATH] = "MyAwesomeGame";
};
