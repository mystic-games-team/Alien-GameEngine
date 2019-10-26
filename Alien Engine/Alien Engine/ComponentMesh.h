#pragma once

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Geometry/AABB.h"

class ComponentMesh : public Component {
public:

	ComponentMesh(GameObject* attach);
	virtual ~ComponentMesh();

	void DrawPolygon();
	void DrawOutLine();
	void DrawMesh();
	void DrawVertexNormals();
	void DrawFaceNormals();
	void DrawInspector();

	void Reset();
	void SetComponent(Component* component);

	AABB GenerateAABB();
	AABB GetOBB();
	AABB GetGlobalAABB();


	AABB local_aabb;
	AABB obb; 
	AABB global_aabb;


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

	bool view_mesh = false;
	bool wireframe = false;
	bool view_vertex_normals = false;
	bool view_face_normals = false;
};