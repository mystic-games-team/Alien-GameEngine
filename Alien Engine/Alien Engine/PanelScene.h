#pragma once

#include "Panel.h"

class PanelScene : public Panel {

public:

	PanelScene(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelScene();

	void PanelLogic();

public:
	float width = 960;
	float height = 540;

	float lastHeight = 540;

	float posX = 0;
	float posY = 0;
};

