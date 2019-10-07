#pragma once

#include "Module.h"
#include "Globals.h"
#include "Objects.h"
#include "Primitive.h"

#include <vector>

class ModuleObjects : public Module
{
public:
	ModuleObjects(bool start_enabled = true);
	virtual ~ModuleObjects();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void LoadConfig(JSONfilepack*& config);
	void SaveConfig(JSONfilepack*& config);

	// Subdivions only for Sphere and Rock, and extra only for Rock seed
	Primitive* CreatePrimitive(const PrimitiveType& type, const float& position_x = 0, const float& position_y = 0, const float& position_z = 0, const uint & subdivions = 5, const uint & seed = 0, const uint & slices = 10, const uint & slacks = 10, const float& radious = 0.5);

	void ChangeWireframeMode();
	void ChangeViewMeshMode();

	void DeleteAllObjects();

public:
	std::vector<Object*> objects;

	bool wireframe_mode = false;
	bool view_mesh_mode = true;
	uint vertex_normal_length = 5;
	uint face_normal_length = 5;
	bool draw_vertex_normals = true;
	bool draw_face_normals = true;

};