#pragma once

#include "Panel.h"

class PanelConfig : public Panel {

public:

	PanelConfig(const std::string& panel_name, const std::vector<SDL_Scancode>& shortcuts);
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

