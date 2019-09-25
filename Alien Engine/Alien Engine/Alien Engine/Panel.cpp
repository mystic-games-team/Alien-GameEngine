#include "Panel.h"

Panel::Panel(const std::string& panel_name, const std::vector<SDL_Scancode>&shortcuts)
{
	this->panel_name = panel_name;
	this->shortcuts = shortcuts;
}

Panel::~Panel()
{
}

const std::string& Panel::GetName()
{
	return panel_name;
}

void Panel::ChangeEnable()
{
	enabled = !enabled;
}

bool Panel::ShortCutClicked()
{
	return false;
}

void Panel::PanelLogic()
{
}



bool Panel::IsEnabled()
{
	return enabled;
}


