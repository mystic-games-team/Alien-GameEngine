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
#include "Parson/parson.h"
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
	JSON_Object* config = nullptr;
	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;
	bool quit = false;

public:

	Application();
	~Application();

	bool LoadConfig();
	bool SaveConfig();

	bool Init();
	update_status Update();
	bool CleanUp();

	void QuitApp();

	void OpenWebsite(const std::string& website);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	JSON_Object* LoadJSONFile(const std::string &path);
};

extern Application* App;