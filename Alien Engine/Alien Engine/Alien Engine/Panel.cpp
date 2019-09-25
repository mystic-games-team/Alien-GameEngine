#include "Panel.h"
#include "ModuleInput.h"


Panel::Panel(const std::string& panel_name, const std::vector<SDL_Scancode>&shortcuts)
{
	this->panel_name = panel_name;

	std::vector<SDL_Scancode> shortcut = shortcuts;
	std::vector<SDL_Scancode>::iterator item = shortcut.begin();
	for (; item != shortcut.end(); ++item) {
		this->shortcuts.push_back(new ShortCuts((*item)));
	}

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

bool Panel::ShortCutClicked(Application*& app)
{
	bool ret = true;

	bool shortcut_can_be_done_again = false;

	std::vector<ShortCuts*>::iterator item = shortcuts.begin();
	for (; item != shortcuts.end(); ++item) { // update the states
		if ((*item) != nullptr) {
			switch ((*item)->shortcut_state) {
			case ShortCutState::WAITING_SHORTCUT:
				if (app->input->GetKey((*item)->shortcut_code) == KEY_REPEAT)
					(*item)->shortcut_state = ShortCutState::CLICKED;
				break;
			case ShortCutState::CLICKED:
				if (app->input->GetKey((*item)->shortcut_code) == KEY_UP)
					(*item)->shortcut_state = ShortCutState::WAITING_SHORTCUT;
				break;
			case ShortCutState::SHORTCUT_DONE:
				if (app->input->GetKey((*item)->shortcut_code) == KEY_UP)
					shortcut_can_be_done_again = true;
				break;
			default:
				break;
			}
		}
	}
	item = shortcuts.begin();
	if (shortcut_can_be_done_again) { // states returns to WAITING_SHORTCUT
		ret = false;
		for (; item != shortcuts.end(); ++item) {
			if ((*item) != nullptr) {
				(*item)->shortcut_state = ShortCutState::WAITING_SHORTCUT;
			}
		}
	}
	else { // check if all key of the shortcut has been pressed
		for (; item != shortcuts.end(); ++item) {
			if ((*item) != nullptr && (*item)->shortcut_state != ShortCutState::CLICKED) {
				ret = false;
			}
		}
	}
	if (ret) { // shortcut has been clicked, pass to state SHORTCUT_DONE
		item = shortcuts.begin();
		for (; item != shortcuts.end(); ++item) {
			if ((*item) != nullptr) {
				(*item)->shortcut_state = ShortCutState::SHORTCUT_DONE;
			}
		}
	}
	return ret;
}

void Panel::PanelLogic()
{
}



bool Panel::IsEnabled()
{
	return enabled;
}




