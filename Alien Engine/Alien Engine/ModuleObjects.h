#pragma once

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Primitive.h"
#include "Color.h"
#include <vector>

class ModuleObjects : public Module
{
public:
	ModuleObjects(bool start_enabled = true);
	virtual ~ModuleObjects();

	bool Init();
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

	void ChangeEnableGrid();
	void ChangeEnableNormalVertex();
	void ChangeEnableNormalFace();

	void SetNewSelectedObject(GameObject* selected);
	const GameObject* GetSelectedObject() const;
	void DeselectObject();

public:

	GameObject* base_game_object = nullptr;


	bool wireframe_mode = false;
	bool view_mesh_mode = true;
	uint vertex_normal_length = 1;
	uint face_normal_length = 1;
	bool draw_vertex_normals = false;
	bool draw_face_normals = false;

	Color mesh_color{ 1,1,1,255 };
	int mesh_line_width = 1;

	bool allow_grid = true;

	Color vertex_n_color{ 0,1,1, };
	Color face_n_color{ 1,0,1, };

	int face_n_width = 1;
	int vertex_n_width = 1;

private:

	GameObject* game_object_selected = nullptr;
};