#pragma once

#include "Panel.h"
#include "GameObject.h"

class PanelHierarchy : public Panel {

public:

	PanelHierarchy(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelHierarchy();

	void PanelLogic();

private:

	void PrintNode(GameObject* node);

};
