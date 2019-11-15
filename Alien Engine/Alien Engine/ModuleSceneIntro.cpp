#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "Parson/parson.h"
#include "glew/include/glew.h"
#include "Shapes.h"
#include "ModuleObjects.h"

#include "MathGeoLib/include/MathGeoLib.h"

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

	App->camera->fake_camera->frustum.pos = { 4,3,5 };
	App->camera->fake_camera->Look(float3(0, 0, 0));

	GameObject* camera_1 = App->objects->CreateEmptyGameObject(App->objects->base_game_object);
	App->objects->camera = new ComponentCamera(camera_1);
	camera_1->AddComponent(App->objects->camera);

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


	return UPDATE_CONTINUE;
}
