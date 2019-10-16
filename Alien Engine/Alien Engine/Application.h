#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleObjects.h"
#include "ModuleUI.h"
#include <list>
#include "j1PerfTimer.h"
#include "JSONfilepack.h"
#include "ShortCutManager.h"
#include "ModuleImporter.h"
#include "ModuleFileSystem.h"

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleUI* ui = nullptr;
	ModuleImporter* importer = nullptr;
	ShortCutManager* shortcut_manager = nullptr;
	ModuleObjects* objects = nullptr;
	ModuleFileSystem* file_system = nullptr;

	bool fps_cap = true;
	uint16_t framerate_cap;
	int fps_limit = 30;
	ImGuiTextBuffer log_string; 
	

private:
	JSONfilepack* config = nullptr;
	JSONfilepack* layout = nullptr;

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

	JSONfilepack* GetJSONLayout();

	bool FileExists(const std::string& name);

	bool Init();
	update_status Update();
	bool CleanUp();

	bool StringCmp(const char* str1, const char* str2);

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