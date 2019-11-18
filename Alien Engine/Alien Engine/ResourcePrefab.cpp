#include "ResourcePrefab.h"
#include "Application.h"
#include "ModuleObjects.h"

ResourcePrefab::ResourcePrefab()
{
}

ResourcePrefab::~ResourcePrefab()
{
}

bool ResourcePrefab::CreateMetaData(GameObject* object)
{
	path = std::string(ASSETS_PREFAB_FOLDER + std::string(object->GetName()) + ".alienfab");

	JSON_Value* value = json_value_init_object();
	JSON_Object* json_object = json_value_get_object(value);
	json_serialize_to_file_pretty(value, path.data());

	if (value != nullptr && json_object != nullptr) {
		// save ID in assets
		JSONfilepack* prefab_scene = new JSONfilepack(path.data(), json_object, value);
		prefab_scene->StartSave();
		ID = App->resources->GetRandomID();
		prefab_scene->SetString("Prefab.ID", std::to_string(ID));
		prefab_scene->FinishSave();
		delete prefab_scene;
		// save prefab in library
		meta_data_path = std::string(LIBRARY_PREFAB_FOLDER + std::to_string(ID) + ".alienPrefab");

		JSON_Value* prefab_value = json_value_init_object();
		JSON_Object* prefab_object = json_value_get_object(prefab_value);
		json_serialize_to_file_pretty(prefab_value, meta_data_path.data());

		if (prefab_value != nullptr && prefab_object != nullptr) {
			JSONfilepack* prefab = new JSONfilepack(meta_data_path.data(), prefab_object, prefab_value);
			prefab->StartSave();

			JSONArraypack* game_objects = prefab->InitNewArray("Prefab.GameObjects");

			game_objects->SetAnotherNode();

			App->objects->SaveGameObject(object, game_objects, 1);

			prefab->FinishSave();
			delete prefab;

			App->resources->AddResource(this);
		}
	}
	else {
		LOG("Could not load scene, fail when creating the file");
	}

	return true;
}

bool ResourcePrefab::ReadBaseInfo(const char* assets_file_path)
{
	path = std::string(assets_file_path);

	JSON_Value* value = json_value_init_object();
	JSON_Object* json_object = json_value_get_object(value);

	if (value != nullptr && json_object != nullptr) {
		JSONfilepack* meta = new JSONfilepack(path.data(), json_object, value);
		ID = std::stoull(meta->GetString("Prefab.ID"));
	}

	return true;
}


bool ResourcePrefab::DeleteMetaData()
{
	return false;
}

void ResourcePrefab::ConvertToGameObjects()
{
}
