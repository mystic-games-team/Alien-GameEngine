#pragma once

#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
#include "Color.h"

class ResourceMesh;

class __declspec(dllexport) ComponentMesh : public Component {
	friend class ReturnZ;
	friend class CompZ;
	friend class ResourceMesh;
	friend class ComponentLight;
	friend class ComponentMaterial;
	friend class ComponentTransform;
	friend class ComponentCamera;
	friend class GameObject;
	friend class ModuleCamera3D;
	friend class ModuleObjects;
	friend class Gizmos;
	friend class Octree;
	friend class OctreeNode;
	friend class PanelCreateObject;
	friend class PanelRender;
public:

	ComponentMesh(GameObject* attach);
	virtual ~ComponentMesh();

private:

	void DrawPolygon();
	void DrawOutLine();
	void DrawMesh();
	void DrawVertexNormals();
	void DrawFaceNormals();
	bool DrawInspector();
	void DrawGlobalAABB();
	void DrawOBB();

	void Reset();
	void SetComponent(Component* component);

	void RecalculateAABB_OBB();
	const AABB GetGlobalAABB() const;
	const OBB GetOBB() const;

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	AABB GenerateAABB();

private:
	
	ResourceMesh* mesh = nullptr;

	bool view_mesh = false;
	bool wireframe = false;
	bool view_vertex_normals = false;
	bool view_face_normals = false;

	bool draw_AABB = true;
	bool draw_OBB = true;

	AABB local_aabb;
	OBB obb;
	AABB global_aabb;
};