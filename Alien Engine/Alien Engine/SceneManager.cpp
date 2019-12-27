#include "SceneManager.h"
#include "Application.h"
#include "ModuleObjects.h"

int SceneManager::scenes_loaded = 0;

void SceneManager::LoadScene(const char* scene_name)
{
	App->objects->LoadScene(scene_name);
}

int SceneManager::ScenesLoaded()
{
	return scenes_loaded;
}
