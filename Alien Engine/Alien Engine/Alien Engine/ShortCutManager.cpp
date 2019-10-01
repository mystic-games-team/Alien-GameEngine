#include "ShortCutManager.h"
#include "Application.h"
#include "ModuleInput.h"

ShortCutManager::ShortCutManager()
{
}

ShortCutManager::~ShortCutManager()
{
	std::vector<ShortCut*>::iterator item = shortcuts.begin();
	for (; item != shortcuts.end(); ++item) {
		if (*item != nullptr) {
			delete* item;
			*item = nullptr;
		}
	}
	shortcuts.end();
}

void ShortCutManager::UpdateShortCuts()
{
	std::vector<ShortCut*>::iterator item = shortcuts.begin();
	for (; item != shortcuts.end(); ++item) {
		if (*item != nullptr) {
			switch ((*item)->type) {
			case ShortCutType::ONE_KEY:
				if (App->input->GetKey((*item)->key1_down) == KEY_DOWN)
					(*item)->funct();
				break;
			case ShortCutType::TWO_KEYS:
				if (App->input->GetKey((*item)->key1_down) == KEY_DOWN && App->input->GetKey((*item)->key2_repeat) == KEY_REPEAT)
					(*item)->funct();
				break;
			case ShortCutType::COMPLETE:
				if (App->input->GetKey((*item)->key1_down) == KEY_DOWN && (App->input->GetKey((*item)->key2_repeat) == KEY_REPEAT || App->input->GetKey((*item)->key3_repeat_extra) == KEY_REPEAT))
					(*item)->funct();
				break;
			default:
				LOG("ShortCutTypeUnknown");
				break;
			}
		}
	}
}

ShortCut* ShortCutManager::AddShortCut(const SDL_Scancode& key1_down, std::function<void()> funct, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
{
	ShortCut* new_shortcut = nullptr;
	new_shortcut = new ShortCut(key1_down, funct, key2_repeat, key3_repeat_extra);
	if (new_shortcut != nullptr) {
		shortcuts.push_back(new_shortcut);
		return new_shortcut;
	}
	else {
		LOG("Fail adding a shortcut");
		return nullptr;
	}
}

