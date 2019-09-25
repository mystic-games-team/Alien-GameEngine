#include "Panel.h"

Panel::Panel(const std::string& panel_name, const std::vector<SDL_Scancode>&shortcuts)
{
	this->panel_name = panel_name;
	this->shortcuts = shortcuts;
}

Panel::~Panel()
{
}

bool Panel::ShortCutClicked()
{
	return false;
}

void Panel::PanelLogic()
{
}

void Panel::Enable()
{
	enabled = true;
}

void Panel::Disable()
{
	enabled = false;
}

bool Panel::IsEnabled()
{
	return enabled;
}


