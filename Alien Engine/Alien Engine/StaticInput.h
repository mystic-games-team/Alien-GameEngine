#pragma once
#include "SDL/include/SDL_scancode.h"
enum KEY_STATE;

class __declspec(dllexport) Input {
public:

	enum {
		KEY_IDLE = 0,
		KEY_DOWN,
		KEY_REPEAT,
		KEY_UP
	};

	static KEY_STATE GetKey(const SDL_Scancode& code);
};
