#pragma once

#include "Resource_.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "GameObject.h"
#include "Color.h"

class ResourceTexture;

class ResourceMesh : public Resource {

	friend class ModuleImporter;
	friend class ResourceModel;

public:

	ResourceMesh();
	virtual ~ResourceMesh();

	bool CreateMetaData(const u64& force_id = 0);
	bool ReadBaseInfo(const char* assets_file_path);

	void FreeMemory();
	bool LoadMemory();

	bool DeleteMetaData();

	void ConvertToGameObject(std::vector<std::pair<u64, GameObject*>>* objects_created);

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

	bool is_primitive = false;
	bool is_custom = true;
private:

	std::string parent_name;
	u64 texture_id = 0;
	uint family_number = 0;

	ResourceTexture* texture = nullptr;

	float3 pos = { 0,0,0 };
	float3 scale = { 1,1,1 };
	Quat rot = { 0,0,0,0 };

	Color material_color;

};