#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ResourceTexture.h"

ResourceMesh::ResourceMesh() : Resource()
{
	type = ResourceType::RESOURCE_MESH;
}

ResourceMesh::~ResourceMesh()
{
	if (index != nullptr)
		RELEASE_ARRAY(index);
	if (vertex != nullptr)
		RELEASE_ARRAY(vertex);
	if (normals != nullptr)
		RELEASE_ARRAY(normals);
	if (uv_cords != nullptr)
		RELEASE_ARRAY(uv_cords);
	if (center_point_normal != nullptr)
		RELEASE_ARRAY(center_point_normal);
	if (center_point != nullptr)
		RELEASE_ARRAY(center_point);

	glDeleteBuffers(1, &id_vertex);
	glDeleteBuffers(1, &id_index);
	glDeleteBuffers(1, &id_normals);
	glDeleteBuffers(1, &id_uv);

}

bool ResourceMesh::CreateMetaData()
{
	if (parent_name.empty()) {
		parent_name.assign("null");
	}

	ID = App->resources->GetRandomID();

	meta_data_path = std::string(LIBRARY_MESHES_FOLDER + std::to_string(ID) + ".alienMesh");

	JSON_Value* value = json_value_init_object();
	JSON_Object* object = json_value_get_object(value);
	json_serialize_to_file_pretty(value, meta_data_path.data());

	if (value != nullptr && object != nullptr) {

		JSONfilepack* meta = new JSONfilepack(meta_data_path, object, value);

		meta->StartSave();

		// names
		meta->SetString("Mesh.ParentName", parent_name.data());
		meta->SetString("Mesh.Name", name.data());
		meta->SetNumber("Mesh.FamilyNumber", family_number);

		// texture path
		meta->SetBoolean("Mesh.HasTexture", (texture != nullptr) ? true : false);

		if (texture != nullptr)
			meta->SetString("Mesh.Texture", texture->GetAssetsPath());

		// transformations
		// pos
		meta->SetFloat3("Mesh.Position", pos);

		// scale
		meta->SetFloat3("Mesh.Scale", scale);

		// rot
		meta->SetQuat("Mesh.Rotation", rot);

		// ranges
		meta->SetNumber("Mesh.NumVertex", num_vertex);
		meta->SetNumber("Mesh.NumIndex", num_index);
		meta->SetNumber("Mesh.NumFaces", num_faces);

		// vertex
		meta->SetNumberArray("Mesh.Vertex", vertex, num_vertex * 3);
		
		// index
		meta->SetUintArray("Mesh.Index", index, num_index);

		// set a bolean to know if it has normals
		meta->SetBoolean("Mesh.HasNormals", (normals != nullptr ? true : false));

		// normals
		if (normals != nullptr) {
			// normals
			meta->SetNumberArray("Mesh.Normals", normals, num_vertex * 3);

			// center point
			meta->SetNumberArray("Mesh.CenterPoint", center_point, num_faces * 3);

			// center point normals
			meta->SetNumberArray("Mesh.CenterPointNormals", center_point_normal, num_faces * 3);
		}

		// set a bolean to know if it has uv
		meta->SetBoolean("Mesh.HasUV", (uv_cords != nullptr ? true : false));

		// uv
		if (uv_cords != nullptr) {
			meta->SetNumberArray("Mesh.UV", uv_cords, num_vertex * 3);
		}

		meta->FinishSave();

		delete meta;
		return true;
	}
	else {
		LOG("Error creating meta with path %s", meta_data_path.data());
		return false;
	}
}

bool ResourceMesh::ReadMetaData(const char* path)
{	
	bool ret = true;

	ID = std::stoull(App->file_system->GetBaseFileName(path));

	JSON_Value* value = json_parse_file(path);
	JSON_Object* object = json_value_get_object(value);

	if (value != nullptr && object != nullptr)
	{
		JSONfilepack* meta = new JSONfilepack(path, object, value);

		meta_data_path = std::string(path); 

		// names
		parent_name = meta->GetString("Mesh.ParentName");
		name = meta->GetString("Mesh.Name");

		family_number = meta->GetNumber("Mesh.FamilyNumber");

		// texture path
		bool has_texture = meta->GetBoolean("Mesh.HasTexture");

		if (has_texture) { // just save a string with the texture name and do GetResourceByName
			texture = App->importer->LoadTextureFile(meta->GetString("Mesh.Texture"));
		}

		// transformations
		// pos
		pos = meta->GetFloat3("Mesh.Position");

		// scale
		scale = meta->GetFloat3("Mesh.Scale");

		// rot
		rot = meta->GetQuat("Mesh.Rotation");

		// ranges
		num_vertex = meta->GetNumber("Mesh.NumVertex");
		num_index = meta->GetNumber("Mesh.NumIndex");
		num_faces = meta->GetNumber("Mesh.NumFaces");

		// vertex
		vertex = (float*)meta->GetNumberArray("Mesh.Vertex");

		// index
		index = meta->GetUintArray("Mesh.Index");

		// normals
		bool has_normals = meta->GetBoolean("Mesh.HasNormals");

		if (has_normals) {
			// normals
			normals = (float*)meta->GetNumberArray("Mesh.Normals");

			// center point
			center_point = (float*)meta->GetNumberArray("Mesh.CenterPoint");

			// center point normal
			center_point_normal = (float*)meta->GetNumberArray("Mesh.CenterPointNormals");
		}

		// uv
		bool has_uv = meta->GetBoolean("Mesh.HasUV");

		if (has_uv) {
			uv_cords = (float*)meta->GetNumberArray("Mesh.UV");
		}

		InitBuffers();
		App->resources->AddResource(this);

		delete meta;
	}
	else {
		LOG("Error loading %s", path);
		ret = false;
	}

	return ret;
}

bool ResourceMesh::DeleteMetaData()
{
	remove(meta_data_path.data());

	std::vector<Resource*>::iterator position = std::find(App->resources->resources.begin(), App->resources->resources.end(), static_cast<Resource*>(this));
	if (position != App->resources->resources.end())
		App->resources->resources.erase(position);

	delete this;

	return true;
}

void ResourceMesh::ConvertToGameObject(std::vector<GameObject*>* objects_created)
{
	// get the parent
	GameObject* obj = nullptr;

	// if parent name is not null, search the parent name
	if (!App->StringCmp(parent_name.data(), "null") && objects_created != nullptr) {
		std::vector<GameObject*>::iterator item = objects_created->begin();
		for (; item != objects_created->end(); ++item) {
			if (*item != nullptr && App->StringCmp((*item)->GetName(), name.data())) {
				obj = new GameObject((*item));
				break;
			}
		}
		objects_created->push_back(obj);
	}
	else if (objects_created != nullptr) { // parent name == null so, the parent must be the one created before
		obj = new GameObject(App->objects->base_game_object->children.back());
		objects_created->push_back(obj);
	}
	else { // if objects created == nullptr then parent must be the root
		obj = new GameObject(App->objects->base_game_object);
	}
	obj->SetName(name.data());

	obj->AddComponent(new ComponentTransform(obj, pos, rot, scale));

	ComponentMesh* mesh = new ComponentMesh(obj);
		
	mesh->mesh = this;
	mesh->RecalculateAABB_OBB();

	obj->AddComponent(mesh);

	ComponentMaterial* material = new ComponentMaterial(obj);

	material->texture = texture;

	obj->AddComponent(material);

}

void ResourceMesh::InitBuffers()
{
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *num_vertex * 3,
		&vertex[0], GL_STATIC_DRAW);

	// index
	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_index,
		&index[0], GL_STATIC_DRAW);

	if (uv_cords != nullptr) {
		// UV
		glGenBuffers(1, &id_uv);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_uv);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * num_vertex * 3,
			&uv_cords[0], GL_STATIC_DRAW);
	}

	if (normals != nullptr) {
		// normals
		glGenBuffers(1, &id_normals);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_normals);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * num_vertex * 3,
			&normals[0], GL_STATIC_DRAW);
	}
}
