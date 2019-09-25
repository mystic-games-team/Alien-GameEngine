#pragma once

#include "Panel.h"

class PanelConfig : public Panel {

public:

	PanelConfig(const std::string& panel_name, const std::vector<SDL_Scancode>& shortcuts);
	virtual ~PanelConfig();

	void PanelLogic();
};

