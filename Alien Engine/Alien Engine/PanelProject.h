#pragma once

#include "Panel.h"

struct FileNode;


class PanelProject : public Panel {

public:

	PanelProject(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelProject();
	void PanelLogic();

private:

	void PrintDirectoryNodes(FileNode* node);
	void DeleteNodes(FileNode* node);

	int colum_width[2];


	FileNode* assets;
};
