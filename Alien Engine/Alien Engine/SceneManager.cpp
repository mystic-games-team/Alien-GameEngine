#include "SceneManager.h"
#include "Application.h"
#include "ModuleObjects.h"

int SceneManager::scenes_loaded = 0;

void SceneManager::LoadScene(const char* path)
{
	App->objects->LoadScene(path);
}

int SceneManager::ScenesLoaded()
{
	return scenes_loaded;
}
