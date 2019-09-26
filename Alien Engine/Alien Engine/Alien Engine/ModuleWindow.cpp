#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	name.assign("Window");
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(fullscreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(borderless)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(full_desktop)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width * SCREEN_SIZE, height * SCREEN_SIZE, flags);
		SDL_SetWindowBrightness(window, brightness);
		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::LoadConfig(JSON_Object*& config)
{
	width = json_object_dotget_number(config, "Configuration.Window.Width");
	height = json_object_dotget_number(config, "Configuration.Window.Height");
	brightness = json_object_dotget_number(config, "Configuration.Window.Brightness");
	fullscreen = json_object_dotget_boolean(config, "Configuration.Window.Fullscreen");
	full_desktop = json_object_dotget_boolean(config, "Configuration.Window.Fulldesktop");
	resizable = json_object_dotget_boolean(config, "Configuration.Window.Resizable");
	borderless = json_object_dotget_boolean(config, "Configuration.Window.Borderless");
	style = json_object_dotget_number(config, "Configuration.Window.Style.Type");
	style_color.r = json_object_dotget_number(config, "Configuration.Window.Style.ColorR");
	style_color.b = json_object_dotget_number(config, "Configuration.Window.Style.ColorB");
	style_color.g = json_object_dotget_number(config, "Configuration.Window.Style.ColorG");
	style_color.a = json_object_dotget_number(config, "Configuration.Window.Style.ColorA");
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}