#pragma once

#include "Panel.h"

class PanelLayout : public Panel {

public:

	PanelLayout(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelLayout();
	void PanelLogic();
	void ChangePanel();
private:

	void PanelLayoutEditor();
	void PanelSaveNewLayout();

public:

	bool is_editor_panel = true;

private:

	std::string new_layout_name = "New Layout";
};
