#pragma once

#include "Panel.h"

class PanelInspector : public Panel {

public:

	PanelInspector(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelInspector();
	void PanelLogic();

};
