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

	path = std::string(LIBRARY_MESHES_FOLDER + name + ".alienMesh");

	JSON_Value* value = json_value_init_object();
	JSON_Object* object = json_value_get_object(value);
	json_serialize_to_file_pretty(value, path.data());

	if (value != nullptr && object != nullptr) {

		JSONfilepack* meta = new JSONfilepack(path, object, value);

		meta->StartSave();

		// names
		meta->SetString("Mesh.ParentName", parent_name.data());
		meta->SetString("Mesh.Name", name.data());

		// texture path
		meta->SetBoolean("Mesh.HasTexture", (texture != nullptr) ? true : false);

		if (texture != nullptr)
			meta->SetString("Mesh.Texture", texture->path);

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
		LOG("Error creating meta with path %s", path.data());
	}
}

bool ResourceMesh::ReadMetaData(char* path)
{	
	bool ret = true;

	JSON_Value* value = json_parse_file(path);
	JSON_Object* object = json_value_get_object(value);

	if (value != nullptr && object != nullptr)
	{
		JSONfilepack* meta = new JSONfilepack(path, object, value);

		// names
		parent_name = meta->GetString("Mesh.ParentName");
		name = meta->GetString("Mesh.Name");

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
		App->importer->InitMeshBuffers(this);
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


	obj->AddComponent(new ComponentTransform(obj, pos, rot, scale));

	ComponentMesh* mesh = new ComponentMesh(obj);

	mesh->num_faces = num_faces;
	mesh->num_index = num_index;
	mesh->num_vertex = num_vertex;
	// TODO: set the buffer if is not setted
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
