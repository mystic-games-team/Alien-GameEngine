#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "imgui.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	return ret;
}

// Load assets
bool ModuleUI::CleanUp()
{
	LOG("Unloading UI scene");

	return true;
}

// Update
update_status ModuleUI::Update(float dt)
{



	return UPDATE_CONTINUE;
}

