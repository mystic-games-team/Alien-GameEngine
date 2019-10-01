#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "imgui/examples/imgui_impl_sdl.h"

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
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
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

	mouse_x_motion = mouse_y_motion = 0;

	bool first_key = true;
	first_key_pressed = SDL_SCANCODE_UNKNOWN;
	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;
			case SDL_KEYDOWN:
			if (first_key) {
				first_key = false;
				first_key_pressed = e.key.keysym.scancode;
			}
			break;
			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;
			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	if(quit == true)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
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
		sprintf_s(text, 60, "MOUSE: %i - %s \n", key, state_string);
	else
		sprintf_s(text, 60, "KEYBOARD: %i - %s \n", key, state_string);
	
	static char repeat[60] = "";

	if (strcmp(repeat, text))
		input.appendf(text);

	strcpy(repeat, text);
}
