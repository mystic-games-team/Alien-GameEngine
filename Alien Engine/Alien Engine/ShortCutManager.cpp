#include "ShortCutManager.h"
#include "Application.h"
#include "ModuleInput.h"
#include "PanelConfig.h"
#include <algorithm>

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
	if (App->ui->panel_config->IsEnabled())
		ChangeKey();
	ShortCutClicked();
}

ShortCut* ShortCutManager::AddShortCut(const char* order_name, const SDL_Scancode& key1_down, std::function<void()> funct, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
{
	ShortCut* new_shortcut = nullptr;
	new_shortcut = new ShortCut(order_name, key1_down, funct, key2_repeat, key3_repeat_extra);
	if (new_shortcut != nullptr) {
		shortcuts.push_back(new_shortcut);
		return new_shortcut;
	}
	else {
		LOG("Fail adding a shortcut");
		return nullptr;
	}
}

std::vector<ShortCut*> ShortCutManager::GetShortCuts()
{
	return shortcuts;
}

void ShortCutManager::OrderShortCuts()
{
	std::sort(shortcuts.begin(), shortcuts.end(), ShortCutManager::SortByType);
}

bool ShortCutManager::SortByType(const ShortCut* shortcut1, const ShortCut* shortcut2)
{
	return (uint)shortcut1->type < (uint)shortcut2->type;
}

void ShortCutManager::ChangeKey()
{
	std::vector<ShortCut*>::iterator item = shortcuts.begin();
	for (; item != shortcuts.end(); ++item) {
		if (*item != nullptr && (*item)->state != ShortCutStateChange::NONE) {
			if (App->input->IsMousePressed()) {
				(*item)->state = ShortCutStateChange::NONE;
			}
			if (App->input->GetFirstKeyPressed() != SDL_SCANCODE_UNKNOWN) {
				switch ((*item)->state) {
				case ShortCutStateChange::WAITING_KEY_REPEAT:
					if (App->input->GetFirstKeyPressed() != (*item)->key1_down && App->input->GetFirstKeyPressed() != (*item)->key3_repeat_extra) {
						(*item)->SetShortcutKeys((*item)->key1_down, App->input->GetFirstKeyPressed(), (*item)->key3_repeat_extra);
						(*item)->state = ShortCutStateChange::NONE;
						if ((*item)->type == ShortCutType::ONE_KEY)
							(*item)->type = ShortCutType::TWO_KEYS;
						OrderShortCuts();
						return;
					}
					break;
				case ShortCutStateChange::WAITING_KEY_DOWN:
					if (App->input->GetFirstKeyPressed() != (*item)->key2_repeat && App->input->GetFirstKeyPressed() != (*item)->key3_repeat_extra) {
						(*item)->SetShortcutKeys(App->input->GetFirstKeyPressed(), (*item)->key2_repeat, (*item)->key3_repeat_extra);
						(*item)->state = ShortCutStateChange::NONE;
						OrderShortCuts();
						return;
					}
					break;
				case ShortCutStateChange::WAITING_EXTRA_KEY_REPEAT:
					if (App->input->GetFirstKeyPressed() != (*item)->key1_down && App->input->GetFirstKeyPressed() != (*item)->key2_repeat) {
						(*item)->SetShortcutKeys((*item)->key1_down, (*item)->key2_repeat, App->input->GetFirstKeyPressed());
						(*item)->state = ShortCutStateChange::NONE;
						if ((*item)->type == ShortCutType::TWO_KEYS)
							(*item)->type = ShortCutType::COMPLETE;
						OrderShortCuts();
						return;
					}
					break;
				default:
					LOG("ShortCutTStateChange UNKNOWN");
					break;
				}
			}
		}
	}
}

void ShortCutManager::ShortCutClicked()
{
	std::vector<ShortCut*>::iterator item = shortcuts.begin();
	for (; item != shortcuts.end(); ++item) {
		if (*item != nullptr) {
			if ((*item)->state == ShortCutStateChange::NONE) {
				switch ((*item)->type) {
				case ShortCutType::ONE_KEY:
					if (App->input->GetKey((*item)->key1_down) == KEY_DOWN) {
						(*item)->funct();
						return;
					}
					break;
				case ShortCutType::TWO_KEYS:
					if (App->input->GetKey((*item)->key1_down) == KEY_DOWN && App->input->GetKey((*item)->key2_repeat) == KEY_REPEAT) {
						(*item)->funct();
						return;
					}
					break;
				case ShortCutType::COMPLETE:
					if (App->input->GetKey((*item)->key1_down) == KEY_DOWN && (App->input->GetKey((*item)->key2_repeat) == KEY_REPEAT || App->input->GetKey((*item)->key3_repeat_extra) == KEY_REPEAT)) {
						(*item)->funct();
						return;
					}
					break;
				default:
					LOG("ShortCutTypeUnknown");
					break;
				}
			}
		}
	}
}

const char* ShortCut::GetShortcutName()
{
	switch (type) {
	case ShortCutType::ONE_KEY:
		sprintf_s(shortcut_char, 50, "%s", SDL_GetScancodeName(key1_down));
		break;
	case ShortCutType::TWO_KEYS:
		sprintf_s(shortcut_char, 50, "%s + %s", SDL_GetScancodeName(key2_repeat), SDL_GetScancodeName(key1_down));
		break;
	case ShortCutType::COMPLETE:
		sprintf_s(shortcut_char, 50, "%s / %s + %s", SDL_GetScancodeName(key2_repeat), SDL_GetScancodeName(key3_repeat_extra), SDL_GetScancodeName(key1_down));
		break;
	default:
		LOG("ShortCutType in get name UNKNOWN")
		break;
	}
	return shortcut_char;
}

void ShortCut::SetShortcutKeys(const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
{
	this->key1_down = key1_down;
	this->key2_repeat = key2_repeat;
	this->key3_repeat_extra = key3_repeat_extra;

	if (key3_repeat_extra != SDL_SCANCODE_UNKNOWN)
		type = ShortCutType::COMPLETE;
	else if (key2_repeat != SDL_SCANCODE_UNKNOWN)
		type = ShortCutType::TWO_KEYS;
	else
		type = ShortCutType::ONE_KEY;

	name = GetShortcutName();
}

const char* ShortCut::GetNameScancodes()
{
	return name;
}

const char* ShortCut::GetNameOrder()
{
	return order_name;
}

const char* ShortCut::GetKeyDownName()
{
	if (state != ShortCutStateChange::WAITING_KEY_DOWN)
		return SDL_GetScancodeName(key1_down);
	else
		return "Click Key";
}

const char* ShortCut::GetKeyRepeatName()
{
	if (state != ShortCutStateChange::WAITING_KEY_REPEAT) {
		if (key2_repeat != SDL_SCANCODE_UNKNOWN)
			return SDL_GetScancodeName(key2_repeat);
		else
			return "No Key On";
	}
	else {
		return "Click Key";
	}
}

const char* ShortCut::GetExtraKeyRepeatName()
{
	if (state != ShortCutStateChange::WAITING_EXTRA_KEY_REPEAT) {
		if (key3_repeat_extra != SDL_SCANCODE_UNKNOWN)
			return SDL_GetScancodeName(key3_repeat_extra);
		else
			return "No Key On";
	}
	else
		return "Click Key";
	
}

const SDL_Scancode& ShortCut::GetScancode(const uint& index)
{
	SDL_Scancode ret = SDL_SCANCODE_UNKNOWN;
	switch (index) {
	case 0: {
		ret = key1_down;
		break; }
	case 1: {
		ret = key2_repeat;
		break; }
	case 2: {
		ret = key3_repeat_extra;
		break; }
	default: {
		LOG("ShortCutIndex not valid %i", index);
		break; }
	}
	return ret;
}

const SDL_Scancode* ShortCut::GetScancodesArray() 
{
	SDL_Scancode codes[3];
	codes[0] = key1_down;
	codes[1] = key2_repeat;
	codes[2] = key3_repeat_extra;
	return codes;
}
