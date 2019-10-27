#pragma once

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
#include "Color.h"

class ResourceMesh;

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
	void DrawGlobalAABB();

	void Reset();
	void SetComponent(Component* component);

	AABB GetGlobalAABB();

private:

	AABB GenerateAABB();

public:
	
	ResourceMesh* mesh = nullptr;

	bool view_mesh = false;
	bool wireframe = false;
	bool view_vertex_normals = false;
	bool view_face_normals = false;

	bool draw_AABB = true;
	bool draw_OBB = true;

private:

	AABB local_aabb;
	OBB obb;
	AABB global_aabb;
};