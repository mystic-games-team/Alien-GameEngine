#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

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

	// char
	char save_char[MAX_META_DATA_CHAR];
	
	// header
	uint ranges[2] = { num_vertex, num_index };
	uint size = sizeof(ranges) + sizeof(char[MAX_META_DATA_CHAR]) * 2 +sizeof(float) * num_vertex * 3 + sizeof(uint) * num_index;

	char* data = new char[size]; 
	char* cursor = data;

	uint bytes = sizeof(ranges); 
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// parent name
	sprintf_s(save_char, MAX_META_DATA_CHAR, "%s", parent_name.data());
	bytes = sizeof(save_char);
	memcpy(cursor, save_char, bytes);
	cursor += bytes;

	// name
	sprintf_s(save_char, MAX_META_DATA_CHAR, "%s", name.data());
	bytes = sizeof(save_char);
	memcpy(cursor, save_char, bytes);
	cursor += bytes;

	// vertex
	bytes = sizeof(float) * num_vertex * 3;
	memcpy(cursor, vertex, bytes);
	cursor += bytes;

	// index
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

		char* cursor = data;

		uint ranges[2];
		uint bytes = sizeof(ranges);

		// header
		memcpy(ranges, cursor, bytes);
		num_vertex = ranges[0];
		num_index = ranges[1];
		cursor += bytes;

		// char
		char load_char[MAX_META_DATA_CHAR];

		// parent name
		bytes = sizeof(load_char);
		memcpy(load_char, cursor, bytes);
		cursor += bytes;
		parent_name = std::string(load_char);

		// name
		bytes = sizeof(load_char);
		memcpy(load_char, cursor, bytes);
		cursor += bytes;
		name = std::string(load_char);

		// Load vertex
		bytes = sizeof(float) * num_vertex * 3;
		vertex = new float[num_vertex * 3];
		memcpy(vertex, cursor, bytes);

		// Load index
		cursor += bytes;
		bytes = sizeof(uint) * num_index;
		index = new uint[num_index];
		memcpy(index, cursor, bytes);
	
		App->resources->AddResource(this);
		delete[] data;
	}
	else {
		ret = false;
		LOG("Error loading %s", path);
	}

	return ret;
}

void ResourceMesh::ConvertToGameObject(std::vector<GameObject*>* objects_created)
{
	// get the parent
	GameObject* obj = nullptr;
	if (!App->StringCmp(parent_name.data(), "null") && objects_created != nullptr) {
		std::vector<GameObject*>::iterator item = objects_created->begin();
		for (; item != objects_created->end(); ++item) {
			if (*item != nullptr && App->StringCmp((*item)->GetName(), name.data())) {
				obj = new GameObject((*item));
				break;
			}
		}
	}
	else {
		obj = new GameObject(App->objects->base_game_object->children.back());
	}
	objects_created->push_back(obj);
	obj->SetName(name.data());


	obj->AddComponent(new ComponentTransform(obj, { 0,0,0 }, { 0,0,0,0 }, { 1,1,1 }));

	ComponentMesh* mesh = new ComponentMesh(obj);

	mesh->num_faces = num_faces;
	mesh->num_index = num_index;
	mesh->num_vertex = num_vertex;

	mesh->id_index = id_index;
	mesh->id_uv = id_uv;
	mesh->id_normals = id_normals;
	mesh->id_vertex = id_vertex;

	mesh->vertex = vertex;
	mesh->normals = normals;
	mesh->uv_cords = uv_cords;
	mesh->center_point = center_point;
	mesh->center_point_normal = center_point_normal;
	mesh->index = index;

	obj->AddComponent(mesh);

	ComponentMaterial* material = new ComponentMaterial(obj);

	material->texture = texture;

	obj->AddComponent(material);

}
