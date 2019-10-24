#pragma once

#include "Resource_.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

struct Texture;

class ResourceMesh : public Resource {

	friend class ModuleImporter;

public:

	ResourceMesh();
	virtual ~ResourceMesh();

	char* CreateMetaData();

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

	float3 aabb_min = { 0,0,0 };
	float3 aabb_max = { 0,0,0 };

	Texture* texture = nullptr;

	float3 pos = { 0,0,0 };
	float3 scale = { 0,0,0 };
	Quat rot = { 0,0,0,0 };

private:

	std::string parent_name;

	// transforms...

};