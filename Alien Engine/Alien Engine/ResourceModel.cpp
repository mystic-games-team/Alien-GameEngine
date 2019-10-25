#include "ResourceModel.h"
#include "Globals.h"
#include "ResourceMesh.h"
#include "ModuleFileSystem.h"
#include "Application.h"
#include <algorithm>

ResourceModel::ResourceModel() : Resource()
{
	type = ResourceType::RESOURCE_MODEL;
}

ResourceModel::~ResourceModel()
{
}

void ResourceModel::CreateMetaData()
{
	uint num_meshes = meshes_attached.size();

	// file size: num meshes + name + path fbx + path for every mesh
	uint size = sizeof(num_meshes) + sizeof(char[MAX_META_DATA_CHAR]) + sizeof(char[MAX_META_DATA_CHAR]) + sizeof(char[MAX_META_DATA_CHAR]) * num_meshes;

	char* data = new char[size]; // Allocate
	char* cursor = data;

	// save the number of meshes
	uint bytes = sizeof(num_meshes);

	// num meshes
	memcpy(cursor, &num_meshes, bytes);
	cursor += bytes;

	// char
	char save_char[MAX_META_DATA_CHAR];

	// name
	sprintf_s(save_char, MAX_META_DATA_CHAR, "%s", name.data());
	bytes = sizeof(save_char);
	memcpy(cursor, save_char, bytes);
	cursor += bytes;

	// fbx path
	sprintf_s(save_char, MAX_META_DATA_CHAR, "%s", path.data());
	bytes = sizeof(save_char);
	memcpy(cursor, save_char, bytes);
	cursor += bytes;

	std::vector<ResourceMesh*>::iterator item = meshes_attached.begin();
	for (; item != meshes_attached.end(); ++item) {
		if ((*item) != nullptr) {
			(*item)->CreateMetaData();

			sprintf_s(save_char, MAX_META_DATA_CHAR, "%s", (*item)->GetPath());
			memcpy(cursor, save_char, sizeof(save_char));
			cursor += sizeof(save_char);

			LOG("Created alienMesh file %s", (*item)->GetPath());
		}
	}
	original_path = path;
	// Create the file
	App->file_system->SaveUnique(path, data, size, LIBRARY_MODELS_FOLDER, App->file_system->GetBaseFileName(path.data()).data(), ".alien");
	LOG("Created alien file %s", path.data());

	delete[] data;
}

bool ResourceModel::ReadMetaData(char* path)
{
	bool ret = true;

	char* data = nullptr;
	App->file_system->Load(path, &data);

	if (data != nullptr) {
		char* cursor = data;
		this->path = std::string(path);
		// TODO: original path get

		uint num_meshes = 0;
		
		// num meshes
		uint bytes = sizeof(num_meshes);
		memcpy(&num_meshes, cursor, bytes);
		cursor += bytes;

		// char
		char load_char[MAX_META_DATA_CHAR];

		// name
		bytes = sizeof(load_char);
		memcpy(load_char, cursor, bytes);
		cursor += bytes;
		name = std::string(load_char);

		// fbx path
		bytes = sizeof(load_char);
		memcpy(load_char, cursor, bytes);
		cursor += bytes;
		original_path = std::string(load_char);

		for (uint i = 0; i < num_meshes; ++i) {

			// get the name of the nodes path
			bytes = sizeof(load_char);
			memcpy(load_char, cursor, bytes);
			cursor += bytes;

			// read the mesh meta data
			ResourceMesh* r_mesh = new ResourceMesh();
			if (r_mesh->ReadMetaData(load_char)) {
				meshes_attached.push_back(r_mesh);
			}
			else {
				LOG("Error loading %s", load_char);
				delete r_mesh;
			}
		}
		App->resources->AddResource(this);
		delete[] data;
	}
	else {
		ret = false;
		LOG("Error loading %s", path);
	}

	return ret;
}

void ResourceModel::ConvertToGameObjects()
{
	// TODO: save & load the family number

	std::sort(meshes_attached.begin(), meshes_attached.end(), ResourceModel::SortByFamilyNumber);

	if (meshes_attached.size() > 1) { // needs an empty gameobject
		
		// create the parent
		GameObject* parent = App->objects->CreateEmptyGameObject(nullptr);
		parent->SetName(name.data());

		// vector to find the parents
		std::vector<GameObject*> objects_created;
		objects_created.push_back(parent);

		std::vector<ResourceMesh*>::iterator item = meshes_attached.begin();
		for (; item != meshes_attached.end(); ++item) {
			if (*item != nullptr) {
				(*item)->ConvertToGameObject(objects_created);
			}
		}
		objects_created.clear();
	}
	else { 
		//meshes_attached.back()->ConvertToGameObject(std::vector<GameObject*>());
	}
}

bool ResourceModel::SortByFamilyNumber(const ResourceMesh* mesh1, const ResourceMesh* mesh2)
{
	return mesh1->family_number < mesh2->family_number;
}
