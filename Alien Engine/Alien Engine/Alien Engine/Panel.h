#pragma once

#include "imgui/imgui.h"
#include "SDL/include/SDL.h"
#include <string>
#include <vector>

class Panel {

public:

	Panel(const std::string& panel_name, const std::vector<SDL_Scancode>&shortcuts);
	virtual ~Panel();

	virtual void PanelLogic();
	void Enable();
	void Disable();
	bool IsEnabled();
	bool ShortCutClicked();
	
private:

	bool enabled = false;

public:

	std::string panel_name;
	std::vector<SDL_Scancode>shortcuts;

};
