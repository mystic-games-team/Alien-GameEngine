#pragma once

#include "Panel.h"

class PanelConsole : public Panel {

public:

	PanelConsole(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelConsole();

	void PanelLogic();
	void OnPanelDesactive();

public:

	bool clear_on_play = true;

private:

	bool game_console = true;
	bool scroll_x = true;
	bool scroll_y = true;

};

