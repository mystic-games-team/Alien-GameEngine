#include "Application.h"
#include "Parson/parson.h"

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	scene_intro = new ModuleSceneIntro();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	ui = new ModuleUI();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);

	// Scenes
	AddModule(scene_intro);
	AddModule(ui);
	// Renderer last!
	AddModule(renderer3D);

}

Application::~Application()
{
	json_value_free(json_object_get_value(config, "Configuration/Configuration.json"));
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend())
	{
		delete(*item);
		++item;
	}

	list_modules.clear();

}

bool Application::LoadConfig()
{
	bool ret = true;


	fps_cap = json_object_dotget_boolean(config, "Configuration.Application.CapFPS");
	fps_limit = json_object_dotget_number(config, "Configuration.Application.LimitFPS");
	if (fps_limit > 0)
	{
		framerate_cap = 1000 / fps_limit;
	}

	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end())
	{
		(*item)->LoadConfig(config);
		++item;
	}

	return true;
}

bool Application::SaveConfig()
{
	bool ret = true;

	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end())
	{
		(*item)->SaveConfig(config);
		++item;
	}

	return true;
}

bool Application::Init()
{
	bool ret = true;
	config = LoadJSONFile("Configuration/Configuration.json");
	if (config != nullptr)
		LoadConfig();
	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		++item;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		++item;
	}
	
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;
	dt = frame_time.ReadSec();
	frame_time.Start();
	ptimer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (last_sec_frame_time.Read() > 1000)
	{
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
		last_sec_frame_time.Start();
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	unsigned __int32 last_frame_ms = frame_time.Read();
	unsigned __int32 frames_on_last_update = prev_last_sec_frame_count;

	if (framerate_cap > 0 && last_frame_ms < framerate_cap && fps_cap)
	{
		j1PerfTimer time;
		float delaytimestart = time.ReadMs();
		SDL_Delay(framerate_cap - last_frame_ms);
		float delaytimefinish = time.ReadMs();
		LOG("We waited for %i milliseconds and got back in %.6f", framerate_cap - last_frame_ms, delaytimefinish - delaytimestart);
	}

	ui->FramerateRegister((float)prev_last_sec_frame_count, (float)(framerate_cap+last_frame_ms));
}

JSON_Object* Application::LoadJSONFile(const std::string& path)
{
	JSON_Value* value = json_parse_file(path.data());
	JSON_Object* object = json_value_get_object(value);

	if (value == nullptr || object == nullptr)
	{
		LOG("Error loading %s", path);
	}

	return object;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		++item;
	}
	if (quit)
		ret = UPDATE_STOP;
	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	SaveConfig();
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		++item;
	}
	return ret;
}

void Application::QuitApp()
{
	quit = true;
}

void Application::OpenWebsite(const std::string& website)
{
	ShellExecuteA(NULL, "open", website.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}