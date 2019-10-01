#pragma once

#include "SDL/include/SDL_scancode.h"
#include <functional>
#include <vector>

enum class ShortCutType {
	COMPLETE,
	TWO_KEYS,
	ONE_KEY,

	NONE
};

struct ShortCut {

	ShortCut(const SDL_Scancode& key1_down, std::function<void()> funct, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN) {
		this->key1_down = key1_down;
		this->key2_repeat = key2_repeat;
		this->key3_repeat_extra = key3_repeat_extra;
		this->funct = funct;

		if (key3_repeat_extra != SDL_SCANCODE_UNKNOWN)
			type = ShortCutType::COMPLETE;
		else if (key2_repeat != SDL_SCANCODE_UNKNOWN)
			type = ShortCutType::TWO_KEYS;
		else
			type = ShortCutType::ONE_KEY;

		name = GetShortcutName();
	}

	// functions
	void SetShortcutKeys(const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	const char* GetName();

	// variables
	SDL_Scancode key1_down = SDL_SCANCODE_UNKNOWN;
	SDL_Scancode key2_repeat = SDL_SCANCODE_UNKNOWN; 
	SDL_Scancode key3_repeat_extra = SDL_SCANCODE_UNKNOWN;
	std::function<void()> funct = nullptr;
	ShortCutType type = ShortCutType::NONE;
	

	private:
	// functions
	const char* GetShortcutName();

	// variables
	const char* name = nullptr;

};

class ShortCutManager {

public:

	ShortCutManager();
	~ShortCutManager();

	void UpdateShortCuts();
	ShortCut* AddShortCut(const SDL_Scancode& key1_down, std::function<void()> funct, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);

private:

	std::vector<ShortCut*> shortcuts;


};
