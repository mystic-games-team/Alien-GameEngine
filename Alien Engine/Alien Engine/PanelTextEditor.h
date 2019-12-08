#pragma once

#include "Panel.h"
#include "TextEdit/TextEditor.h"

class PanelTextEditor : public Panel {

public:

	PanelTextEditor(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelTextEditor();

	void PanelLogic();

	void SetFile(const char* file);

	bool IsFocused() const;

private:

	void GetAllFiles();

public:

	TextEditor text_editor;
	std::string file_to_edit;
	std::string file_name;
	
private:

	std::vector<std::string> all_scripts_files;
	bool is_focused = false;
};

#pragma once

