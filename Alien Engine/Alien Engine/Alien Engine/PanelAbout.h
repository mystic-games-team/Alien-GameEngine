#pragma once

#include "Panel.h"

class PanelAbout : public Panel {

public:

	PanelAbout(const std::string& panel_name, const std::vector<SDL_Scancode>& shortcuts);
	virtual ~PanelAbout();
	void PanelLogic();

	void OnPanelDesactive();
};
