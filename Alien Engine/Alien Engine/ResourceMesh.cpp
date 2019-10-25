#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleFileSystem.h"

ResourceMesh::ResourceMesh() : Resource()
{
	type = ResourceType::RESOURCE_MESH;
}

ResourceMesh::~ResourceMesh()
{
}

void ResourceMesh::CreateMetaData()
{
	if (parent_name.empty()) {
		parent_name.assign("null");
	}

	const char* node_names[2] = { parent_name.data(), name.data() };
	
	// header
	uint ranges[2] = { num_vertex, num_index };
	uint size = sizeof(ranges) + sizeof(float) * num_vertex * 3 + sizeof(uint) * num_index;

	char* data = new char[size]; 
	char* cursor = data;

	uint bytes = sizeof(ranges); 
	memcpy(cursor, ranges, bytes);

	// vertex
	cursor += bytes; 
	bytes = sizeof(float) * num_vertex * 3;
	memcpy(cursor, vertex, bytes);

	// index
	cursor += bytes;
	bytes = sizeof(uint) * num_index;
	memcpy(cursor, index, bytes);

	// save
	std::string output;
	App->file_system->SaveUnique(output, data, size, LIBRARY_MESHES_FOLDER, name.data(), ".alienMesh");

	path = output.data();

	delete[] data;
}

bool ResourceMesh::ReadMetaData(char* path)
{	
	bool ret = true;

	char* data = nullptr;
	App->file_system->Load(path, &data);

	if (data != nullptr) {
		this->path = std::string(path);

		uint ranges[2];
		uint bytes = sizeof(ranges);

		memcpy(ranges, data, bytes);
		num_vertex = ranges[0];
		num_index = ranges[1];

		// Load vertex
		data += bytes;
		bytes = sizeof(float) * num_vertex * 3;
		vertex = new float[num_vertex * 3];
		memcpy(vertex, data, bytes);

		// Load index
		data += bytes;
		bytes = sizeof(uint) * num_index;
		index = new uint[num_index];
		memcpy(index, data, bytes);
		
		App->resources->AddResource(this);
		delete[] data;
	}
	else {
		ret = false;
		LOG("Error loading %s", path);
	}

	return ret;
}

void ResourceMesh::ConvertToGameObject(std::vector<GameObject*>& objects_created)
{
	//GameObject* ret = new GameObject(parent);


}
