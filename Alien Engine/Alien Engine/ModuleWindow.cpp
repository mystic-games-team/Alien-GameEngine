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
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		
		window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_ICON_WIDTH, WINDOW_ICON_HEIGHT, flags);
		if(window == NULL)
		{
			LOG_ENGINE("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, 0);
			screen_surface = SDL_LoadBMP("Configuration/EngineTextures/Logo_Name.bmp");
			texture = SDL_CreateTextureFromSurface(renderer, screen_surface);
			SDL_RenderCopy(renderer, texture, NULL, NULL);

			SDL_RenderPresent(renderer);
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
	if (ImGui::GetCurrentContext() != nullptr) {
		App->ui->ChangeStyle(style);
	}
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

void ModuleWindow::SetBorderless(bool borderless)
{
	this->borderless = borderless;
	SDL_SetWindowBordered(window, (SDL_bool)borderless);
}

void ModuleWindow::SetFullScreen(bool fullscreen)
{
	this->fullscreen = fullscreen;
	if (fullscreen) {
		full_desktop = false;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}
	else {
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowBordered(window, (SDL_bool)borderless);
		SDL_SetWindowResizable(window, (SDL_bool)resizable);
	}
}

void ModuleWindow::SetFullDesktop(bool fulldesktop)
{
	this->full_desktop = fulldesktop;
	if (fulldesktop) {
		fullscreen = false;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else {
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowBordered(window, (SDL_bool)borderless);
		SDL_SetWindowResizable(window, (SDL_bool)resizable);
	}
}

void ModuleWindow::SetResizable(bool resizable)
{
	this->resizable = resizable;
	SDL_SetWindowResizable(window, (SDL_bool)resizable);
}

void ModuleWindow::IncreaseBar()
{
	SDL_Rect r;
	r.x = BAR_BEGIN_POS + segment_width * current_division;
	r.y = 280;
	r.w = segment_width;
	r.h = 15;
	++current_division;
	SDL_SetRenderDrawColor(renderer, 0, 170, 0, 255);
	SDL_RenderFillRect(renderer, &r);
	SDL_RenderPresent(renderer);
	SDL_Delay(50);
}

bool ModuleWindow::CreateCoreWindow()
{
	bool ret = true;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(screen_surface);
	SDL_DestroyRenderer(renderer);

	SDL_SetWindowPosition(window, 0, 0);
	SDL_SetWindowSize(window, width, height);
	SDL_SetWindowBordered(window, SDL_bool(true));

	if (start_maximized) {
		int display_index = SDL_GetWindowDisplayIndex(window);
		if (display_index < 0) {
			return false;
		}

		SDL_Rect usable_bounds;
		if (0 != SDL_GetDisplayUsableBounds(display_index, &usable_bounds)) {
			return false;
		}
		int bar_size = 0;
		SDL_GetWindowBordersSize(window, &bar_size, nullptr, nullptr, nullptr);
		SDL_SetWindowPosition(window, usable_bounds.x, usable_bounds.y + bar_size - 1);
		SDL_SetWindowSize(window, usable_bounds.w, usable_bounds.h - bar_size + 1);
	}
	else if (!borderless)
	{
		SDL_SetWindowBordered(window, SDL_bool(false));
	}

	if (fullscreen)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}

	if (resizable)
	{
		SDL_SetWindowResizable(window, (SDL_bool)true);
	}

	if (full_desktop)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	SDL_GetWindowSize(window, &width, &height);

	screen_surface = SDL_GetWindowSurface(window);
	App->renderer3D->OnResize(width, height);
	return ret;
}

