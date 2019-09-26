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
	void LoadConfig(JSON_Object*& config);
	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	int width = 1200;
	int height = 675;
	float brightness = 1.0;
	bool fullscreen = false;
	bool full_desktop = false;
	bool borderless = false;
	bool resizable = true;
	int style = 1;
	Color style_color{ 100,100,100,100 };
	char* window_name = "Alien Engine";
	char* organitzation_name = "UPC CITM";

};

#endif // __ModuleWindow_H__