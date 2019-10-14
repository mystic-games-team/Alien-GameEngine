#pragma once

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Color.h"
#include <vector>

enum class PrimitiveType
{
	CUBE,
	SPHERE_ALIEN,
	ROCK,
	DODECAHEDRON,
	OCTAHEDRON,
	TORUS,
	ICOSAHEDRON,

	UNKONWN
};

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

	void CreateBasePrimitive(PrimitiveType type);

	void ChangeWireframeMode();
	void ChangeViewMeshMode();

	void DeleteAllObjects();

	void ChangeEnableGrid();
	void ChangeEnableNormalVertex();
	void ChangeEnableNormalFace();

	void SetNewSelectedObject(GameObject* selected);
	GameObject* GetSelectedObject();
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

	bool outline = false;
	uint parent_line_width = 1;
	uint no_child_line_width = 1;
	Color parent_outline_color{ 1,1,1 };
	Color no_child_outline_color{ 1,1,1 };

private:

	GameObject* game_object_selected = nullptr;
};