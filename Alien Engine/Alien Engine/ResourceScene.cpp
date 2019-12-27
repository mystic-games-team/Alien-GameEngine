#include "ResourceScene.h"
#include "Application.h"

ResourceScene::ResourceScene() : Resource()
{
	type = ResourceType::RESOURCE_SCENE;
}

ResourceScene::~ResourceScene()
{
}

bool ResourceScene::CreateMetaData(const u64& force_id)
{
	if (force_id != 0) {
		ID = force_id;
	}
	else {
		ID = App->resources->GetRandomID();
	}

	App->file_system->NormalizePath(path);
	name = App->file_system->GetBaseFileName(path.data());

	App->file_system->CreateNewFile(path.data());

	JSON_Value* value = json_value_init_object();
	JSON_Object* json_object = json_value_get_object(value);

	std::string meta_path = std::string(App->file_system->GetPathWithoutExtension(path) + "_meta.alien");

	json_serialize_to_file_pretty(value, meta_path.data());

	if (value != nullptr && json_object != nullptr) {

		JSONfilepack* file = new JSONfilepack(meta_path, json_object, value);
		file->StartSave();
		file->SetString("Meta.ID", std::to_string(ID));
		file->FinishSave();
		delete file;
	}

	meta_data_path = LIBRARY_SCENES_FOLDER + std::to_string(ID) + ".alienScene";
	App->file_system->CreateNewFile(meta_data_path.data());
	App->resources->AddResource(this);
	return true;
}

bool ResourceScene::ReadBaseInfo(const char* assets_file_path)
{
	// TODO: ha dhaver dos fitxers de scene en els assets, si no es troba el de la library que es torni a crear
	path = std::string(assets_file_path);
	name = App->file_system->GetBaseFileName(path.data());

	std::string meta_path = std::string(App->file_system->GetPathWithoutExtension(path) + "_meta.alien");

	ID = App->resources->GetIDFromAlienPath(meta_path.data());

	if (ID != 0) {
		meta_data_path = LIBRARY_SCENES_FOLDER + std::to_string(ID) + ".alienScene";

		if (!App->file_system->Exists(meta_data_path.data())) {
			App->file_system->Copy(meta_path.data(), meta_data_path.data());
		}

		App->resources->AddResource(this);
		return true;
	}
	return false;
}

bool ResourceScene::DeleteMetaData()
{
	remove(meta_data_path.data());
	return true;
}
