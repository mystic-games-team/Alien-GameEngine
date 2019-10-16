#include "Panel.h"
#include "ModuleInput.h"


Panel::Panel(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
{
	this->panel_name = panel_name;
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

	if (!enabled)
		OnPanelDesactive();
}

bool Panel::IsEnabled()
{
	return enabled;
}

void Panel::SetEnable(const bool& enabled)
{
	this->enabled = enabled;

	if (!this->enabled)
		OnPanelDesactive();
}

const std::string Panel::GetPanelName() const
{
	return panel_name;
}




