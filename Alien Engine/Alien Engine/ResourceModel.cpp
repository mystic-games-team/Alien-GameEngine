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

	// file size
	uint size = sizeof(num_meshes) + sizeof(char*) * num_meshes;

	char* data = new char[size]; // Allocate
	char* cursor = data;

	// save the number of meshes
	uint bytes = sizeof(num_meshes);

	memcpy(cursor, &num_meshes, bytes);
	cursor += bytes;

	std::vector<ResourceMesh*>::iterator item = meshes_attached.begin();
	for (; item != meshes_attached.end(); ++item) {
		if ((*item) != nullptr) {
			(*item)->CreateMetaData();
			const char* mesh_path[1] = { (*item)->GetPath() };
			memcpy(cursor, mesh_path, sizeof(mesh_path));
			cursor += sizeof(mesh_path);
			LOG("Created alienMesh file %s", mesh_path[0]);
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

		this->path = std::string(path);
		// TODO: original path get

		uint num_meshes = 0;
		
		uint bytes = sizeof(num_meshes);
		memcpy(&num_meshes, data, bytes);
		data += bytes;

		for (uint i = 0; i < num_meshes; ++i) {

			// get the name of the nodes path
			char* mesh_path[1];
			bytes = sizeof(mesh_path);
			memcpy(mesh_path, data, bytes);
			data += bytes;

			// read the mesh meta data
			ResourceMesh* r_mesh = new ResourceMesh();
			if (r_mesh->ReadMetaData(mesh_path[0])) {
				meshes_attached.push_back(r_mesh);
			}
			else {
				LOG("Error loading %s", mesh_path[0]);
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
		GameObject* parent = App->objects->CreateEmptyGameObject(nullptr);



	}
	else { 

	}
}

bool ResourceModel::SortByFamilyNumber(const ResourceMesh* mesh1, const ResourceMesh* mesh2)
{
	return mesh1->family_number < mesh2->family_number;
}
