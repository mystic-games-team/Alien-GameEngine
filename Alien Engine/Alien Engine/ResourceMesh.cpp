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
}

void ResourceMesh::CreateMetaData()
{
	if (parent_name.empty()) {
		parent_name.assign("null");
	}
	// TODO: random number
	meta_data_path = std::string(LIBRARY_MESHES_FOLDER + name + ".alienMesh");

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
		meta->SetArrayNumber("Mesh.Position", pos.x);
		meta->SetArrayNumber("Mesh.Position", pos.y);
		meta->SetArrayNumber("Mesh.Position", pos.z);

		// scale
		meta->SetArrayNumber("Mesh.Scale", scale.x);
		meta->SetArrayNumber("Mesh.Scale", scale.y);
		meta->SetArrayNumber("Mesh.Scale", scale.z);

		// rot
		meta->SetArrayNumber("Mesh.Rotation", rot.x);
		meta->SetArrayNumber("Mesh.Rotation", rot.y);
		meta->SetArrayNumber("Mesh.Rotation", rot.z);
		meta->SetArrayNumber("Mesh.Rotation", rot.w);

		// ranges
		meta->SetNumber("Mesh.NumVertex", num_vertex);
		meta->SetNumber("Mesh.NumIndex", num_index);
		meta->SetNumber("Mesh.NumFaces", num_faces);

		// vertex
		for (uint i = 0; i < num_vertex * 3; ++i) {
			meta->SetArrayNumber("Mesh.Vertex", vertex[i]);
		}

		// index
		for (uint i = 0; i < num_index; ++i) {
			meta->SetArrayNumber("Mesh.Index", index[i]);
		}

		// set a bolean to know if it has normals
		meta->SetBoolean("Mesh.HasNormals", (normals != nullptr ? true : false));

		// normals
		if (normals != nullptr) {
			// normals
			for (uint i = 0; i < num_vertex * 3; ++i) {
				meta->SetArrayNumber("Mesh.Normals", normals[i]);
			}
			// center point
			for (uint i = 0; i < num_faces * 3; ++i) {
				meta->SetArrayNumber("Mesh.CenterPoint", center_point[i]);
			}
			// center point normals
			for (uint i = 0; i < num_faces * 3; ++i) {
				meta->SetArrayNumber("Mesh.CenterPointNormals", center_point_normal[i]);
			}
		}

		// set a bolean to know if it has uv
		meta->SetBoolean("Mesh.HasUV", (uv_cords != nullptr ? true : false));

		// uv
		if (uv_cords != nullptr) {
			for (uint i = 0; i < num_vertex * 3; ++i) {
				meta->SetArrayNumber("Mesh.UV", uv_cords[i]);
			}
		}


		meta->FinishSave();

		delete meta;
	}
	else {
		LOG("Error creating meta with path %s", meta_data_path.data());
	}
}

bool ResourceMesh::ReadMetaData(const char* path)
{	
	bool ret = true;

	JSON_Value* value = json_parse_file(path);
	JSON_Object* object = json_value_get_object(value);

	if (value != nullptr && object != nullptr)
	{
		JSONfilepack* meta = new JSONfilepack(path, object, value);

		meta_data_path = std::string(meta_data_path);

		// names
		parent_name = meta->GetString("Mesh.ParentName");
		name = meta->GetString("Mesh.Name");

		family_number = meta->GetNumber("Mesh.FamilyNumber");

		// texture path
		bool has_texture = meta->GetBoolean("Mesh.HasTexture");

		if (has_texture) {
			texture = App->importer->LoadTextureFile(meta->GetString("Mesh.Texture"));
		}

		// transformations
		// pos
		pos.x = meta->GetArrayNumber("Mesh.Position", 0);
		pos.y = meta->GetArrayNumber("Mesh.Position", 1);
		pos.z = meta->GetArrayNumber("Mesh.Position", 2);

		// scale
		scale.x = meta->GetArrayNumber("Mesh.Scale", 0);
		scale.y = meta->GetArrayNumber("Mesh.Scale", 1);
		scale.z = meta->GetArrayNumber("Mesh.Scale", 2);

		// rot
		rot.x = meta->GetArrayNumber("Mesh.Rotation", 0);
		rot.y = meta->GetArrayNumber("Mesh.Rotation", 1);
		rot.z = meta->GetArrayNumber("Mesh.Rotation", 2);
		rot.w = meta->GetArrayNumber("Mesh.Rotation", 3);

		// ranges
		num_vertex = meta->GetNumber("Mesh.NumVertex");
		num_index = meta->GetNumber("Mesh.NumIndex");
		num_faces = meta->GetNumber("Mesh.NumFaces");

		// vertex
		vertex = new float[num_vertex * 3];
		for (uint i = 0; i < num_vertex * 3; ++i) {
			vertex[i] = meta->GetArrayNumber("Mesh.Vertex", i);
		}

		// index
		index = new uint[num_index];
		for (uint i = 0; i < num_index; ++i) {
			index[i] = meta->GetArrayNumber("Mesh.Index", i);
		}

		// normals
		bool has_normals = meta->GetBoolean("Mesh.HasNormals");

		if (has_normals) {
			// normals
			normals = new float[num_vertex * 3];
			for (uint i = 0; i < num_vertex * 3; ++i) {
				normals[i] = meta->GetArrayNumber("Mesh.Normals", i);
			}

			// center point
			center_point = new float[num_faces * 3];
			for (uint i = 0; i < num_faces * 3; ++i) {
				center_point[i] = meta->GetArrayNumber("Mesh.CenterPoint", i);
			}

			// center point normal
			center_point_normal = new float[num_faces * 3];
			for (uint i = 0; i < num_faces * 3; ++i) {
				center_point_normal[i] = meta->GetArrayNumber("Mesh.CenterPointNormals", i);
			}
		}

		// uv
		bool has_uv = meta->GetBoolean("Mesh.HasUV");

		if (has_uv) {
			uv_cords = new float[num_vertex * 3];
			for (uint i = 0; i < num_vertex * 3; ++i) {
				uv_cords[i] = meta->GetArrayNumber("Mesh.UV", i);
			}
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
