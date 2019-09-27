#pragma once

#include "Panel.h"

class PanelConfig : public Panel {

public:

	PanelConfig(const std::string& panel_name, const std::vector<SDL_Scancode>& shortcuts);
	virtual ~PanelConfig();

	void PanelLogic();

	void FramerateInfo(float frames, float ms);

public:
	std::vector<float> fps_log;
	std::vector<float> ms_log;
};

