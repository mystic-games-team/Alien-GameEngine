#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"

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
	LOG_ENGINE("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG_ENGINE("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if (start_maximized) {
			flags |= SDL_WINDOW_MAXIMIZED;
		}

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
		
		window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width * SCREEN_SIZE, height * SCREEN_SIZE, flags);
		if(window == NULL)
		{
			LOG_ENGINE("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
			//SDL_SetWindowBrightness(window, brightness);
		}
	}
	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG_ENGINE("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::LoadConfig(JSONfilepack*& config)
{
	width = config->GetNumber("Configuration.Window.Width");
	height = config->GetNumber("Configuration.Window.Height");
	brightness = config->GetNumber("Configuration.Window.Brightness");
	fullscreen = config->GetBoolean("Configuration.Window.Fullscreen");
	full_desktop = config->GetBoolean("Configuration.Window.Fulldesktop");
	resizable = config->GetBoolean("Configuration.Window.Resizable");
	borderless = config->GetBoolean("Configuration.Window.Borderless");
	window_name = (char*)config->GetString("Configuration.Application.Name");
	organitzation_name = (char*)config->GetString("Configuration.Application.Organitzation");
	style = config->GetNumber("Configuration.Window.StyleType");
	start_maximized = config->GetBoolean("Configuration.Window.StartMax");
	if (ImGui::GetCurrentContext() != nullptr)
		App->ui->ChangeStyle(style);
	//SDL_SetWindowTitle(window, window_name);
	//SDL_SetWindowSize(window, width, height);
	//if (fullscreen) {
	//	SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
	//}
	//else {
	//	SDL_SetWindowFullscreen(App->window->window, 0);
	//}
	//if (full_desktop) {
	//	SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	//}
	//else {
	//	SDL_SetWindowFullscreen(App->window->window, 0);
	//}
	//SDL_SetWindowBordered(window, (SDL_bool)!borderless);
	//SDL_SetWindowResizable(window, (SDL_bool)resizable);
	//SDL_SetWindowBrightness(window, brightness);
	//int x, y;
	//SDL_GL_GetDrawableSize(window, &x, &y);
	//SDL_SetWindowSize(window, x, y);
}

void ModuleWindow::SaveConfig(JSONfilepack*& config)
{
	config->SetNumber("Configuration.Window.Width", width);
	config->SetNumber("Configuration.Window.Height", height);
	config->SetNumber("Configuration.Window.Brightness", brightness);
	config->SetBoolean("Configuration.Window.Fullscreen", fullscreen);
	config->SetBoolean("Configuration.Window.Fulldesktop", full_desktop);
	config->SetBoolean("Configuration.Window.Resizable", resizable);
	config->SetBoolean("Configuration.Window.Borderless", borderless);
	config->SetNumber("Configuration.Window.StyleType", style);
	config->SetString("Configuration.Application.Name", window_name);
	config->SetString("Configuration.Application.Organitzation", organitzation_name);
	config->SetBoolean("Configuration.Window.StartMax", start_maximized);
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetWindowName(const char* name)
{
	window_name = (char*)name;
	SDL_SetWindowTitle(window, window_name);
}

void ModuleWindow::SetOrganitzationName(const char* name)
{
	organitzation_name = (char*)name;
}
