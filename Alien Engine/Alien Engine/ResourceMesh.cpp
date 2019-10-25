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

	uint ranges[3] = { num_vertex,num_index,num_faces };

	/////////////////////// num del zie ////////////////// chars in that size //////////////////
	/* size = 2 uints (size parent name & name) + parent name & name string size + ranges + vertex num 
	+ index num + normals num + uv_cords + center point + center face point*/ 
	uint size = sizeof(uint) * 2 + parent_name.size() + name.size() + sizeof(ranges) + (sizeof(float) * num_vertex * 3) 
		+ (sizeof(uint) * num_index) + (sizeof(float) * num_vertex * 3) + (sizeof(float) * num_vertex * 3)
		+ (sizeof(float) * num_faces * 3) + +(sizeof(float) * num_faces * 3);

	char* data = new char[size];
	char* cursor = data;

	uint bytes = 0;

	// save parent name size
	uint parent_size = parent_name.size();
	bytes = sizeof(uint);
	memcpy(cursor, &parent_size, bytes);
	cursor += bytes;

	// save name size
	uint name_size = name.size();
	memcpy(cursor, &name_size, bytes);
	cursor += bytes;

	// save parent name
	bytes = parent_name.size();
	memcpy(cursor, parent_name.data(), bytes);
	cursor += bytes;

	// save name
	bytes = name.size();
	memcpy(cursor, name.data(), bytes);
	cursor += bytes;

	// save ranges
	bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// save vertex
	bytes = sizeof(float) * num_vertex;
	memcpy(cursor, vertex, bytes);
	cursor += bytes;

	// save index
	bytes = sizeof(uint) * num_index;
	memcpy(cursor, index, bytes);
	cursor += bytes;

	// save normals
	if (normals != nullptr) {
		bytes = sizeof(float) * num_vertex * 3;
		memcpy(cursor, normals, bytes);
		cursor += bytes;
	}

	// save uv
	if (uv_cords != nullptr) {
		bytes = sizeof(float) * num_vertex * 3;
		memcpy(cursor, uv_cords, bytes);
		cursor += bytes;
	}

	// center point
	if (center_point != nullptr) {
		bytes = sizeof(float) * num_faces * 3;
		memcpy(cursor, center_point, bytes);
		cursor += bytes;
	}

	// center point normal
	if (center_point_normal != nullptr) {
		bytes = sizeof(float) * num_faces * 3;
		memcpy(cursor, center_point_normal, bytes);
		cursor += bytes;
	}

	// save file
	App->file_system->SaveUnique(path, data, size, LIBRARY_MESHES_FOLDER, name.data(), ".alienMesh");

	delete[] data;
}

bool ResourceMesh::ReadMetaData(char* path)
{	
	bool ret = true;

	char* data = nullptr;
	App->file_system->Load(path, &data);

	if (data != nullptr)
	{
		this->path = path;

		char* cursor = data;

		// get parent name size
		uint parent_name_size = 0;
		uint bytes = sizeof(uint);
		memcpy(&parent_name_size, cursor, bytes);
		cursor += bytes;

		// get name size
		uint name_size = 0;
		memcpy(&name_size, cursor, bytes);
		cursor += bytes;

		// get parent name
		char* parent_name_ = new char[parent_name_size + 1];
		bytes = sizeof(char) * parent_name_size;
		memcpy(parent_name_, cursor, bytes);
		parent_name_[parent_name_size] = '\0';
		cursor += bytes;
		parent_name = std::string(parent_name_);
		delete[] parent_name_;

		// get name
		char* name_ = new char[name_size + 1];
		bytes = sizeof(char) * name_size;
		memcpy(name_, cursor, bytes);
		name_[name_size] = '\0';
		cursor += bytes;
		name = std::string(name_);
		delete[] name_;

		// get ranges 0: vertex 1: index 2: faces
		uint ranges[3];
		bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);
		cursor += bytes;

		num_vertex = ranges[0];
		num_index = ranges[1];
		num_faces = ranges[2];

		// get vertex
		bytes = sizeof(float) * num_vertex * 3;
		vertex = new float[num_vertex * 3];
		memcpy(vertex, cursor, bytes);
		cursor += bytes;

		// get index
		bytes = sizeof(uint) * num_index;
		index = new uint[num_index];
		memcpy(index, cursor, bytes);
		cursor += bytes;

		// get normals
		bytes = sizeof(float) * num_vertex * 3;
		normals = new float[num_vertex * 3];
		memcpy(normals, cursor, bytes);
		cursor += bytes;

		// get uv cords
		bytes = sizeof(float) * num_vertex * 3;
		uv_cords = new float[num_vertex * 3];
		memcpy(uv_cords, cursor, bytes);
		cursor += bytes;

		// get center point
		bytes = sizeof(float) * num_faces * 3;
		center_point = new float[num_faces * 3];
		memcpy(center_point, cursor, bytes);
		cursor += bytes;

		// get center normal
		bytes = sizeof(float) * num_faces * 3;
		center_point_normal = new float[num_faces * 3];
		memcpy(center_point_normal, cursor, bytes);
		// TODO: know if has normals & uv cords... now if it doesnt have GG
		delete[] data;
	}
	else
		ret = false;

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
