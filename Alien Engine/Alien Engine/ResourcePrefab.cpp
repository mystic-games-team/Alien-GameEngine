#include "ResourcePrefab.h"
#include "Application.h"
#include "ModuleObjects.h"
#include "ComponentTransform.h"


ResourcePrefab::ResourcePrefab()
{
}

ResourcePrefab::~ResourcePrefab()
{
}

bool ResourcePrefab::CreateMetaData(GameObject* object)
{
	path = std::string(ASSETS_PREFAB_FOLDER + std::string(object->GetName()) + ".alienfab");

	std::vector<std::string> files;
	std::vector<std::string> dir;
	App->file_system->DiscoverFiles(ASSETS_PREFAB_FOLDER, files, dir);

	if (!files.empty()) {
		uint num_file = 0;
		for (uint i = 0; i < files.size(); ++i) {
			if (App->StringCmp(files[i].data(), App->file_system->GetBaseFileNameWithExtension(path.data()).data())) {
				++num_file;
				path = std::string(ASSETS_PREFAB_FOLDER + std::string(object->GetName()) + " (" + std::to_string(num_file) + ")" + ".alienfab");
				i = -1;
			}
		}
	}
	JSON_Value* value = json_value_init_object();
	JSON_Object* json_object = json_value_get_object(value);
	json_serialize_to_file_pretty(value, path.data());

	if (value != nullptr && json_object != nullptr) {
		// save ID in assets
		JSONfilepack* prefab_scene = new JSONfilepack(path.data(), json_object, value);
		prefab_scene->StartSave();
		ID = App->resources->GetRandomID();
		prefab_scene->SetString("Meta.ID", std::to_string(ID));
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

			App->objects->SaveGameObject(object, game_objects, 1, true);

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

	ID = App->resources->GetIDFromAlienPath(path.data());

	if (ID != 0) {
		meta_data_path = LIBRARY_PREFAB_FOLDER + std::to_string(ID) + ".alienPrefab";
		App->resources->AddResource(this);
	}

	return true;
}


bool ResourcePrefab::DeleteMetaData()
{
	remove(meta_data_path.data());
	return true;
}

void ResourcePrefab::ConvertToGameObjects()
{
	JSON_Value* value = json_parse_file(meta_data_path.data());
	JSON_Object* object = json_value_get_object(value);

	if (value != nullptr && object != nullptr)
	{
		JSONfilepack* prefab = new JSONfilepack(meta_data_path.data(), object, value);

		JSONArraypack* game_objects = prefab->GetArray("Prefab.GameObjects");

		// first is family number, second parentID, third is array index in the json file
		std::vector<std::tuple<uint, u64, uint>> objects_to_create;

		for (uint i = 0; i < game_objects->GetArraySize(); ++i) {
			uint family_number = game_objects->GetNumber("FamilyNumber");
			u64 parentID = std::stoull(game_objects->GetString("ParentID"));
			objects_to_create.push_back({ family_number,parentID, i });
			game_objects->GetAnotherNode();
		}
		std::sort(objects_to_create.begin(), objects_to_create.end(), ModuleObjects::SortByFamilyNumber);
		game_objects->GetFirstNode();
		std::vector<GameObject*> objects_created;

		std::vector<std::tuple<uint, u64, uint>>::iterator item = objects_to_create.begin();
		for (; item != objects_to_create.end(); ++item) {
			game_objects->GetNode(std::get<2>(*item));
			GameObject* obj = new GameObject();
			if (std::get<0>(*item) == 1) { // family number == 1 so parent is the base game object
				obj->LoadObject(game_objects, App->objects->base_game_object);
			}
			else { // search parent
				std::vector<GameObject*>::iterator objects = objects_created.begin();
				for (; objects != objects_created.end(); ++objects) {
					if ((*objects)->ID == std::get<1>(*item)) {
						obj->LoadObject(game_objects, *objects);
						break;
					}
				}
			}
			obj->is_prefab = true;
			objects_created.push_back(obj);
		}
		GameObject* obj = App->objects->base_game_object->children.back();
		App->objects->SetNewSelectedObject(obj);
		ComponentTransform* transform = (ComponentTransform*)obj->GetComponent(ComponentType::TRANSFORM);
		transform->SetLocalPosition(0, 0, 0);
		delete prefab;
	}
	else {
		LOG("Error loading prefab %s", path.data());
	}
}
