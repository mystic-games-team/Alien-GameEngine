#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui/imgui.h"
#include "SDL/include/SDL_scancode.h"
#include "SDL\include\SDL_haptic.h"
#include "SDL\include\SDL_gamecontroller.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <map>

#define DEAD_ZONE 10000 // 0 - 32767
#define MAX_MOUSE_BUTTONS 5
#define MAX_GAMPAD_BUTTONS 17
#define CONTROLLER_BUTTON_LEFTTRIGGER 15
#define CONTROLLER_BUTTON_RIGHTTRIGGER 16

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

struct GamePad {
	struct Joystick {
		float valueX = 0;
		float valueY = 0;
	};
	int number = 0;
	Joystick joystick_left;
	Joystick joystick_right;
	KEY_STATE controller_buttons[MAX_GAMPAD_BUTTONS];
	SDL_Haptic* haptic = nullptr;
	SDL_GameController* controller = nullptr;
};


class ModuleInput : public Module
{
public:
	
	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	int GetMouseZ() const
	{
		return mouse_z;
	}

	int GetMouseXMotion() const
	{
		return mouse_x_motion;
	}

	int GetMouseYMotion() const
	{
		return mouse_y_motion;
	}
	SDL_Scancode GetFirstKeyPressed() const {
		return first_key_pressed;
	}
	bool IsMousePressed() const {
		return mouse_pressed;
	}

	float3 GetMousePosition();
private:

	void AddInputBuff(const uint& key, const uint& state, const bool& is_mouse = false);

private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	std::map<int, GamePad*> game_pads;
	SDL_Scancode first_key_pressed = SDL_SCANCODE_UNKNOWN;
	bool mouse_pressed = false;
public:

	ImGuiTextBuffer input;

};