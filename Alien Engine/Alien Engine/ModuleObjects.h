#pragma once

#include "Module.h"
#include "Globals.h"
#include "Objects.h"
#include "Primitive.h"
#include "Color.h"
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

	Primitive* CreatePrimitive(const PrimitiveType& type);
	Primitive* CreatePrimitive(const PrimitiveType& type, const float& position_x, const float& position_y, const float& position_z, const uint& subdivisions = 5, const uint & seed = 0, const uint & slices = 5, const uint & slacks = 5, const float& radius = 0.5);

	void ChangeWireframeMode();
	void ChangeViewMeshMode();

	void DeleteAllObjects();

public:
	std::vector<Object*> objects;

	bool wireframe_mode = false;
	bool view_mesh_mode = true;
	uint vertex_normal_length = 5;
	uint face_normal_length = 1;
	bool draw_vertex_normals = true;
	bool draw_face_normals = true;

	Color mesh_color{ 1,1,1,255 };
	int mesh_line_width = 1;

	bool allow_grid = true;
};