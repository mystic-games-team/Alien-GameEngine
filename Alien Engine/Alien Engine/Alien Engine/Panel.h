#pragma once

#include "imgui/imgui.h"
#include "SDL/include/SDL.h"
#include <string>
#include <vector>
#include "Application.h"

enum class ShortCutState {

	CLICKED,
	SHORTCUT_DONE,
	WAITING_SHORTCUT,

	NONE
};

struct ShortCuts {

	ShortCuts(const SDL_Scancode& code) : shortcut_code(code) {}

	SDL_Scancode shortcut_code = SDL_SCANCODE_UNKNOWN;
	ShortCutState shortcut_state = ShortCutState::WAITING_SHORTCUT;
};

class Panel {

public:

	Panel(const std::string& panel_name, const std::vector<SDL_Scancode>&shortcuts);
	virtual ~Panel();

	virtual void PanelLogic();

	const std::string& GetName();
	void ChangeEnable();
	bool IsEnabled();
	bool ShortCutClicked();

private:

	
	std::vector<ShortCuts*>shortcuts;

	
protected:
	std::string panel_name;
	bool enabled = false;
};
