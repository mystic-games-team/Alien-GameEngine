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
	
	uint ranges[2] = { num_vertex, num_index };
	uint size = sizeof(ranges) + sizeof(float) * num_vertex * 3 + sizeof(uint) * num_index;

	char* data = new char[size]; 
	char* cursor = data;

	uint bytes = sizeof(ranges); 
	memcpy(cursor, ranges, bytes);

	cursor += bytes; 
	bytes = sizeof(float) * num_vertex * 3;
	memcpy(cursor, vertex, bytes);

	cursor += bytes;
	bytes = sizeof(uint) * num_index;
	memcpy(cursor, index, bytes);

	std::string output;
	App->file_system->SaveUnique(output, data, size, LIBRARY_MESHES_FOLDER, name.data(), ".alienMesh");

	path = output.data();

	delete[] data;
}

void ResourceMesh::ReadMetaData()
{
}
