#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	ui = new ModuleUI(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	
	// Scenes
	AddModule(scene_intro);
	AddModule(ui);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	std::list<Module*>::iterator item = list_modules.end();

	for (; item != list_modules.begin(); ++item) {
		if ((*item != nullptr)) {
			delete (*item);
			(*item) = nullptr;
		}
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();
	for (; item != list_modules.end(); ++item) {
		if ((*item) != nullptr)
			ret = (*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	for (; item != list_modules.end(); ++item) {
		if ((*item) != nullptr)
			ret = (*item)->Start();
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

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	for (; item != list_modules.end(); ++item) 
	{
		ret = (*item)->PreUpdate(dt);
	}

	item = list_modules.begin();

	for (; item != list_modules.end(); ++item) 
	{
		ret = (*item)->Update(dt);
	}

	item = list_modules.begin();

	for (; item != list_modules.end(); ++item) 
	{
		ret = (*item)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::iterator item = list_modules.end();

	for (; item != list_modules.begin(); ++item) 
	{
		ret = (*item)->CleanUp();
		++item;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}