#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "RandomHelper.h"
#include "Parson/parson.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	name.assign("SceneIntro");
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	if(json_parse_file("ip.json")!=NULL)
		LOG("Readed!");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	//Sphere s({ 0,0,0 }, 4);
	//Sphere f({ 0,0,0 }, 7);
	//if (f.Intersects(s)) {
	//	LOG("INTERSECTION DETECTED!!!");
	//}
	//
	//LOG("RAND %.1f",GetRandomFloatBetweenTwo(-103.3f,1.8f));
	//


	return UPDATE_CONTINUE;
}


