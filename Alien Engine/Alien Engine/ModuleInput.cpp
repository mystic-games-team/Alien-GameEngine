#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "ModuleFileSystem.h"
#include "mmgr/mmgr.h"
#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	name.assign("Input");

	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG_ENGINE("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG_ENGINE("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG_ENGINE("SDL_INIT_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) < 0)
	{
		LOG_ENGINE("SDL_GAMECONTROLLER HAPTIC could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE) {
				keyboard[i] = KEY_DOWN;
				AddInputBuff(i, KEY_DOWN);
			}
			else {
				keyboard[i] = KEY_REPEAT;
				AddInputBuff(i, KEY_REPEAT);
			}
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN) {
				keyboard[i] = KEY_UP;
				AddInputBuff(i, KEY_UP);
			}
			else
				keyboard[i] = KEY_IDLE;
		}



	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE) {
				mouse_buttons[i] = KEY_DOWN;
				AddInputBuff(i, KEY_DOWN, true);
			}
			else {
				mouse_buttons[i] = KEY_REPEAT;
				AddInputBuff(i, KEY_REPEAT, true);
			}
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN) {
				mouse_buttons[i] = KEY_UP;
				AddInputBuff(i, KEY_UP, true);
			}
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}
	
	auto item = game_pads.begin();
	for (; item != game_pads.end(); ++item) {
		for (uint i = 0; i < MAX_GAMPAD_BUTTONS; ++i)
		{
			if ((*item).second != nullptr) {
				if ((*item).second->controller_buttons[i] == KEY_DOWN) {
					(*item).second->controller_buttons[i] = KEY_REPEAT;
				}
				if ((*item).second->controller_buttons[i] == KEY_UP) {
					(*item).second->controller_buttons[i] = KEY_IDLE;
				}
			}
		}
		float value = SDL_GameControllerGetAxis((*item).second->controller, SDL_CONTROLLER_AXIS_RIGHTX);
		if (value < -32767 + DEAD_ZONE) {
			(*item).second->joystick_right.valueX = -1;
		}
		else if (value > 32767 - DEAD_ZONE) {
			(*item).second->joystick_right.valueX = 1;
		}
		else if (value > -DEAD_ZONE && value < DEAD_ZONE) {
			(*item).second->joystick_right.valueX = 0;
		}
		else {
			(*item).second->joystick_right.valueX = value / 32767;
		}

		value = SDL_GameControllerGetAxis((*item).second->controller, SDL_CONTROLLER_AXIS_RIGHTY);
		if (value < -32767 + DEAD_ZONE) {
			(*item).second->joystick_right.valueY = -1;
		}
		else if (value > 32767 - DEAD_ZONE) {
			(*item).second->joystick_right.valueY = 1;
		}
		else if (value > -DEAD_ZONE && value < DEAD_ZONE) {
			(*item).second->joystick_right.valueY = 0;
		}
		else {
			(*item).second->joystick_right.valueY = value / 32767;
		}

		value = SDL_GameControllerGetAxis((*item).second->controller, SDL_CONTROLLER_AXIS_LEFTX);
		if (value < -32767 + DEAD_ZONE) {
			(*item).second->joystick_left.valueX = -1;
		}
		else if (value > 32767 - DEAD_ZONE) {
			(*item).second->joystick_left.valueX = 1;
		}
		else if (value > -DEAD_ZONE && value < DEAD_ZONE) {
			(*item).second->joystick_left.valueX = 0;
		}
		else {
			(*item).second->joystick_left.valueX = value / 32767;
		}

		value = SDL_GameControllerGetAxis((*item).second->controller, SDL_CONTROLLER_AXIS_LEFTY);
		if (value < -32767 + DEAD_ZONE) {
			(*item).second->joystick_left.valueY = -1;
		}
		else if (value > 32767 - DEAD_ZONE) {
			(*item).second->joystick_left.valueY = 1;
		}
		else if (value > -DEAD_ZONE && value < DEAD_ZONE) {
			(*item).second->joystick_left.valueY = 0;
		}
		else {
			(*item).second->joystick_left.valueY = value / 32767;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool first_key = true;
	mouse_pressed = false;
	first_key_pressed = SDL_SCANCODE_UNKNOWN;
	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
#ifndef GAME_VERSION
		ImGui_ImplSDL2_ProcessEvent(&e);
#endif
		switch(e.type)
		{
		case SDL_MOUSEWHEEL: {
			mouse_z = e.wheel.y;
			break; }
		case SDL_KEYDOWN: {
			if (first_key) {
				first_key = false;
				first_key_pressed = e.key.keysym.scancode;
			}
			break; }
		case SDL_MOUSEMOTION: {
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break; }
		case SDL_MOUSEBUTTONDOWN: {
			mouse_pressed = true;
			break; }
		case SDL_QUIT: {
			quit = true;
			break; }
		case SDL_DROPFILE: {
			App->file_system->ManageNewDropFile(e.drop.file);
			SDL_free(e.drop.file);
			break; }
		case SDL_WINDOWEVENT: {
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				App->renderer3D->OnResize(e.window.data1, e.window.data2);
			break; }
		case SDL_CONTROLLERAXISMOTION: {
			if (e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT && e.caxis.value > DEAD_ZONE) {
				game_pads[e.cdevice.which + 1]->controller_buttons[CONTROLLER_BUTTON_RIGHTTRIGGER] = KEY_DOWN;
			}
			if (e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT && e.caxis.value < DEAD_ZONE) {
				game_pads[e.cdevice.which + 1]->controller_buttons[CONTROLLER_BUTTON_RIGHTTRIGGER] = KEY_UP;
			}
			if (e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT && e.caxis.value > DEAD_ZONE) {
				game_pads[e.cdevice.which + 1]->controller_buttons[CONTROLLER_BUTTON_LEFTTRIGGER] = KEY_DOWN;
			}
			if (e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT && e.caxis.value < DEAD_ZONE) {
				game_pads[e.cdevice.which + 1]->controller_buttons[CONTROLLER_BUTTON_LEFTTRIGGER] = KEY_UP;
			}
			break; }
		case SDL_CONTROLLERBUTTONUP: {
			game_pads[e.cdevice.which + 1]->controller_buttons[e.cbutton.button] = KEY_UP;
			break; }
		case SDL_CONTROLLERBUTTONDOWN: {
			game_pads[e.cdevice.which + 1]->controller_buttons[e.cbutton.button] = KEY_DOWN;
			break; }
		case SDL_CONTROLLERDEVICEADDED: {
			SDL_GameController* controller = SDL_GameControllerOpen(e.cdevice.which);
			if (controller != nullptr) {
				if (SDL_JoystickIsHaptic(SDL_GameControllerGetJoystick(controller)) > 0)
				{
					SDL_Haptic* haptic = SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(controller));

					if (haptic != nullptr)
					{
						if (SDL_HapticRumbleInit(haptic) < 0) 
						{
							LOG_ENGINE("Warning: Unable to initialize rumble! SDL Error: %s\n", SDL_GetError());
						}

						if (SDL_HapticRumblePlay(haptic, 0.3f, 1000) < 0)
						{
							LOG_ENGINE("Error when rumbing the controller number %i", e.cdevice.which);
						}
					}
					GamePad* pad = new GamePad();
					pad->controller = controller;
					pad->haptic = haptic;
					pad->number = e.cdevice.which + 1;
					game_pads.emplace(pad->number, pad);
					LOG_ENGINE("Controller %i loaded correctly", pad->number);
				}
				else
				{
					LOG_ENGINE("haptic error! SDL_Error: %s\n", SDL_GetError());
					LOG_ENGINE("haptic error! SDL_Error: %i\n", SDL_JoystickIsHaptic(SDL_GameControllerGetJoystick(controller)));
				}
			}
			else {
				LOG_ENGINE("Error when trying to open the controller number %i", e.cdevice.which);
			}
			break; }
		case SDL_CONTROLLERDEVICEREMOVED: {
			auto item = game_pads.begin();
			for (; item != game_pads.end(); ++item) {
				if ((*item).second != nullptr && (*item).first == e.cdevice.which + 1) {
					SDL_HapticClose((*item).second->haptic);
					(*item).second->haptic = nullptr;
					SDL_GameControllerClose((*item).second->controller);
					(*item).second->controller = nullptr;
					delete (*item).second;
					(*item).second = nullptr;
					game_pads.erase(item);
					break;
				}
			}
			break; }
		}
	}

	if (quit == true) {
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG_ENGINE("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

float3 ModuleInput::GetMousePosition()
{
	return float3(mouse_x, mouse_y, mouse_z);
}

void ModuleInput::AddInputBuff(const uint& key, const uint& state, const bool& is_mouse)
{
	static char text[60];
	const char* state_string = nullptr;
	switch (state) {
	case KEY_DOWN:
		state_string = "KEY_DOWN";
		break;
	case KEY_UP:
		state_string = "KEY_UP";
		break;
	case KEY_REPEAT:
		state_string = "KEY_REPEAT";
		break;
	}

	if (is_mouse)
		sprintf_s(text, 60, "MOUSE: %i - %s \n", SDL_GetScancodeName((SDL_Scancode)key), state_string);
	else
		sprintf_s(text, 60, "KEYBOARD: %s - %s \n", SDL_GetScancodeName((SDL_Scancode)key), state_string);
	
	static char repeat[60] = "";

	if (strcmp(repeat, text))
		input.appendf(text);

	strcpy(repeat, text);
}
