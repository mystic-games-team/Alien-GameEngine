#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "RandomHelper.h"
#include "Parson/parson.h"
#include "glew/include/glew.h"
#include "Shapes.h"
#include "ModuleObjects.h"
#include "Primitive.h"

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

	Primitive* cube=App->objects->CreatePrimitive(PrimitiveType::CUBE);
	cube->SetPosition(5, 0, 5);
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
