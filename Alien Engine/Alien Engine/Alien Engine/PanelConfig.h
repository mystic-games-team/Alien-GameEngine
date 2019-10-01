#pragma once

#include "Panel.h"

class PanelConfig : public Panel {

public:

	PanelConfig(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelConfig();

	void PanelLogic();

	void FramerateInfo(float frames, float ms);
	void OnPanelDesactive();

public:
	std::vector<float> fps_keeper;
	std::vector<float> ms_keeper;

private:

	bool scroll_y = true;

};

