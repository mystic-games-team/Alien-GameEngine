#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleUI.h"

#include <list>

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleUI* ui = nullptr;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;
	bool quit = false;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void QuitApp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};