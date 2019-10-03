#pragma once

#include "Panel.h"

class PanelAbout : public Panel {

public:

	PanelAbout(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelAbout();
	void PanelLogic();

	void OnPanelDesactive();
};
