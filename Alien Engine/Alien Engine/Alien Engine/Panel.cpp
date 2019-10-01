#include "Panel.h"
#include "ModuleInput.h"


Panel::Panel(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
{
	this->panel_name = panel_name;
	
	shortcut = App->shortcut_manager->AddShortCut(key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);

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

const SDL_Scancode& Panel::GetScancodeShortcut(const uint& index)
{
	return SDL_SCANCODE_A;
}


bool Panel::IsEnabled()
{
	return enabled;
}




