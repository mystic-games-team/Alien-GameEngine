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

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
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