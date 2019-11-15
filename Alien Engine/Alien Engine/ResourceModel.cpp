#include "ResourceModel.h"
#include "Globals.h"
#include "ResourceMesh.h"
#include "ModuleFileSystem.h"
#include "Application.h"
#include <algorithm>
#include "ReturnZ.h"

ResourceModel::ResourceModel() : Resource()
{
	type = ResourceType::RESOURCE_MODEL;
}

ResourceModel::~ResourceModel()
{
	std::vector<ResourceMesh*>::iterator item = meshes_attached.begin();
	for (; item != meshes_attached.end(); ++item) {
		if (*item != nullptr) { // aixo passa pq es fa el delete del modul resources i aquest respurce mesh ja la borrat alla
			delete* item;
			*item = nullptr;
		}
	}
	meshes_attached.clear();
}

bool ResourceModel::CreateMetaData()
{	
	ID = App->resources->GetRandomID();

	std::string alien_path = std::string(App->file_system->GetPathWithoutExtension(path) + "_meta.alien").data();

	JSON_Value* alien_value = json_value_init_object();
	JSON_Object* alien_object = json_value_get_object(alien_value);
	json_serialize_to_file_pretty(alien_value, alien_path.data());

	if (alien_value != nullptr && alien_object != nullptr) {

		JSONfilepack* alien = new JSONfilepack(alien_path, alien_object, alien_value);
		alien->StartSave();
		alien->SetString("Meta.ID", std::to_string(ID));
		alien->FinishSave();
		delete alien;

		meta_data_path = std::string(LIBRARY_MODELS_FOLDER) + std::string(std::to_string(ID) + ".alienModel");

		JSON_Value* model_value = json_value_init_object();
		JSON_Object* model_object = json_value_get_object(model_value);
		json_serialize_to_file_pretty(model_value, meta_data_path.data());

		if (model_value != nullptr && model_object != nullptr) {

			JSONfilepack* meta = new JSONfilepack(meta_data_path, model_object, model_value);

			meta->StartSave();

			meta->SetString("Model.Name", name);

			meta->SetNumber("Model.NumMeshes", meshes_attached.size());

			std::string* meshes_paths = new std::string[meshes_attached.size()];

			std::vector<ResourceMesh*>::iterator item = meshes_attached.begin();
			for (; item != meshes_attached.end(); ++item) {
				if ((*item) != nullptr) {
					(*item)->CreateMetaData();

					meshes_paths[item - meshes_attached.begin()] = (*item)->GetLibraryPath();
					LOG("Created alienMesh file %s", (*item)->GetLibraryPath());
				}
			}
			meta->SetArrayString("Model.PathMeshes", meshes_paths, meshes_attached.size());
			delete[] meshes_paths;
			// Create the file
			LOG("Created alien file %s", meta_data_path.data());

			meta->FinishSave();
			delete meta;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		LOG("Error creating meta with path %s", meta_data_path.data());
		return false;
	}
}

bool ResourceModel::ReadBaseInfo(const char* assets_file_path)
{
	bool ret = true;

	path = std::string(assets_file_path);
	meta_data_path = App->file_system->GetPathWithoutExtension(assets_file_path) + "_meta.alien";

	JSON_Value* value = json_parse_file(meta_data_path.data());
	JSON_Object* object = json_value_get_object(value);

	if (value != nullptr && object != nullptr)
	{
		JSONfilepack* meta = new JSONfilepack(meta_data_path, object, value);

		ID = std::stoull(meta->GetString("Meta.ID"));
		
		delete meta;

		// InitMeshes
		std::string library_path = LIBRARY_MODELS_FOLDER + std::to_string(ID) + ".alienModel";
		JSON_Value* mesh_value = json_parse_file(library_path.data());
		JSON_Object* mesh_object = json_value_get_object(mesh_value);

		if (mesh_value != nullptr && mesh_object != nullptr) {

			JSONfilepack* model = new JSONfilepack(library_path, mesh_object, mesh_value);

			int num_meshes = model->GetNumber("Model.NumMeshes");
			name = model->GetString("Model.Name");
			
			std::string* mesh_path = model->GetArrayString("Model.PathMeshes");

			for (uint i = 0; i < num_meshes; ++i) {
				ResourceMesh* r_mesh = new ResourceMesh();
				if (r_mesh->ReadBaseInfo(mesh_path[i].data())) {
					meshes_attached.push_back(r_mesh);
				}
				else {
					LOG("Error loading %s", mesh_path[i].data());
					delete r_mesh;
				}
			}
			delete[] mesh_path;
			delete model;
			App->resources->AddResource(this);
		}
		else {
			ret = false;
		}
	}
	else {
		ret = false;
	}

	return ret;
}

bool ResourceModel::LoadMemory()
{
	bool ret = true;

	std::vector<ResourceMesh*>::iterator item = meshes_attached.begin();
	for (; item != meshes_attached.end(); ++item) {
		if (*item != nullptr) {
			(*item)->LoadMemory();
		}
	}

	return ret;
}

bool ResourceModel::DeleteMetaData()
{
	// TODO: delete here the .alien
	remove(meta_data_path.data());

	std::vector<ResourceMesh*>::iterator item = meshes_attached.begin();
	for (; item != meshes_attached.end(); ++item) {
		if (*item != nullptr) {
			(*item)->DeleteMetaData();
		}
	}
	meshes_attached.clear();

	std::vector<Resource*>::iterator position = std::find(App->resources->resources.begin(), App->resources->resources.end(), static_cast<Resource*>(this));
	if (position != App->resources->resources.end()) 
		App->resources->resources.erase(position);

	delete this;

	return true;
}

void ResourceModel::ConvertToGameObjects()
{
	if (meshes_attached.empty())
		return;

	std::sort(meshes_attached.begin(), meshes_attached.end(), ResourceModel::SortByFamilyNumber);

	if (meshes_attached.size() > 1) { // needs an empty gameobject
		
		// create the parent
		GameObject* parent = App->objects->CreateEmptyGameObject(nullptr, false);
		parent->SetName(name.data());
		
		// vector to find the parents
		std::vector<GameObject*> objects_created;
		objects_created.push_back(parent);

		std::vector<ResourceMesh*>::iterator item = meshes_attached.begin();
		for (; item != meshes_attached.end(); ++item) {
			if (*item != nullptr) {
				(*item)->ConvertToGameObject(&objects_created);
			}
		}
		objects_created.clear();

		// set it selected
		App->objects->SetNewSelectedObject(parent);
		ReturnZ::AddNewAction(ReturnZ::ReturnActions::ADD_OBJECT, parent);
	}
	else { 
		meshes_attached.at(0)->ConvertToGameObject(nullptr);
		App->objects->SetNewSelectedObject(App->objects->base_game_object->children.back());
		ReturnZ::AddNewAction(ReturnZ::ReturnActions::ADD_OBJECT, App->objects->base_game_object->children.back());
	}
	App->camera->Focus();
}

bool ResourceModel::SortByFamilyNumber(const ResourceMesh* mesh1, const ResourceMesh* mesh2)
{
	return mesh1->family_number < mesh2->family_number;
}
