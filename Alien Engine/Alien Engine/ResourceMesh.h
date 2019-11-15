#pragma once

#include "Resource_.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "GameObject.h"

class ResourceTexture;

class ResourceMesh : public Resource {

	friend class ModuleImporter;
	friend class ResourceModel;

public:

	ResourceMesh();
	virtual ~ResourceMesh();

	bool CreateMetaData();
	bool ReadBaseInfo(const char* assets_file_path);
	bool LoadMemory();
	bool DeleteMetaData();

	void ConvertToGameObject(std::vector<GameObject*>* objects_created);

	void InitBuffers();

public:

	// buffers id
	uint id_index = 0;
	uint id_vertex = 0;
	uint id_normals = 0;
	uint id_uv = 0;
	// buffers size
	uint num_index = 0;
	uint num_vertex = 0;
	uint num_faces = 0;
	// buffers
	uint* index = nullptr;
	float* vertex = nullptr;
	float* normals = nullptr;
	float* uv_cords = nullptr;
	float* center_point_normal = nullptr;
	float* center_point = nullptr;

private:

	std::string parent_name;
	std::string texture_name;
	uint family_number = 0;

	ResourceTexture* texture = nullptr;

	float3 pos = { 0,0,0 };
	float3 scale = { 0,0,0 };
	Quat rot = { 0,0,0,0 };

};