#pragma once

#include "Panel.h"

struct FileNode;


class PanelProject : public Panel {

public:

	PanelProject(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelProject();
	void PanelLogic();


public:

	FileNode* current_active_folder = nullptr;

private:

	void PrintDirectoryNodes(FileNode* node);
	void SeeFiles();
	bool MoveToFolder(FileNode* node, bool inside);
	void DeleteNodes(FileNode* node);

	static bool SortByFolder(const FileNode* node1, const FileNode* node2);

	int colum_width[2];

	FileNode* assets = nullptr;

	FileNode* current_active_file = nullptr;
	FileNode go_back_folder;

	bool change_folder = false;
};
