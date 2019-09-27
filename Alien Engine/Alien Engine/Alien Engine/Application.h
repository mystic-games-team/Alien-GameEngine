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
#include "j1PerfTimer.h"
#include "JSONfilepack.h"

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleUI* ui = nullptr;

	bool fps_cap = true;
	uint16_t framerate_cap;
	int fps_limit = 30;
	ImGuiTextBuffer log_string;

private:
	JSONfilepack* config = nullptr;

	std::list<JSONfilepack*> json_files;

	std::list<Module*> list_modules;
	bool quit = false;

	j1PerfTimer			ptimer;
	unsigned __int64				frame_count = 0;
	Timer				startup_time;
	Timer				frame_time;
	Timer				last_sec_frame_time;
	unsigned __int32				last_sec_frame_count = 0;
	unsigned __int32				prev_last_sec_frame_count = 0;
	
	float				dt;

public:

	Application();
	~Application();

	void LoadDefaultConfig();
	void SaveCustomConfig();

	bool FileExists(const std::string& name);

	bool Init();
	update_status Update();
	bool CleanUp();

	void QuitApp();

	void OpenWebsite(const std::string& website);

	JSONfilepack* LoadJSONFile(const std::string& path);
	JSONfilepack* CreateJSONFile(const std::string& path);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();


	bool LoadConfig();
	bool SaveConfig();
};

extern Application* App;