#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Color.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();
	void LoadConfig(JSONfilepack*& config);
	void SaveConfig(JSONfilepack*& config);
	void SetTitle(const char* title);

	void SetWindowName(const char* name);
	void SetOrganitzationName(const char* name);

	void SetBorderless(bool borderless);
	void SetFullScreen(bool fullscreen);
	void SetFullDesktop(bool fulldesktop);

	void SetResizable(bool resizable);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	int width = 0;
	int height = 0;
	float brightness = 0.0f;
	bool fullscreen = false;
	bool full_desktop = false;
	bool borderless = false;
	bool resizable = false;
	bool start_maximized = false;
	int style = 0;
	char* window_name = nullptr;
	char* organitzation_name = nullptr;

};

#endif // __ModuleWindow_H__