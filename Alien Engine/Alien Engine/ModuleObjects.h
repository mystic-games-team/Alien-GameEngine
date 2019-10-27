#pragma once

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Color.h"
#include <vector>
#include <map>

enum class PrimitiveType
{
	CUBE,
	SPHERE_ALIEN,
	ROCK,
	DODECAHEDRON,
	OCTAHEDRON,
	TORUS,
	ICOSAHEDRON,
	KLEIN_BOTTLE,

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

	// config
	void LoadConfig(JSONfilepack*& config);
	void SaveConfig(JSONfilepack*& config);

	// primitives
	void CreateBasePrimitive(PrimitiveType type);

	// poly options
	void ChangeWireframeMode();
	void ChangeViewMeshMode();
	void ChangeEnableNormalVertex();
	void ChangeEnableNormalFace();
	void ChangeEnableAABB();
	
	// enable/disable grid
	void ChangeEnableGrid();

	void DeleteAllObjects();

	// select/disselect objects
	void SetNewSelectedObject(GameObject* selected);
	GameObject* GetSelectedObject();
	void DeselectObject();

	// if parent is nullptr, parent will be the invisible game object
	void CreateEmptyGameObject(GameObject* parent);

	// right click menu, move
	void MoveObjectDown(GameObject* object, bool bottom);
	void MoveObjectUp(GameObject* object, bool top);
	void MoveComponentDown(GameObject* object, Component* component, bool bottom);
	void MoveComponentUp(GameObject* object, Component* component, bool top);
	
	// get object
	GameObject* GetGameObjectByID(const int& id);

	//reparent object in the next preupdate
	void ReparentGameObject(GameObject* object, GameObject* next_parent);

public:

	// root
	GameObject* base_game_object = nullptr;
	Component* component_in_copy = nullptr;

	// wireframe
	bool wireframe_mode = false;

	// mesh
	bool view_mesh_mode = true;
	int mesh_line_width = 1;
	Color mesh_color{ 1,1,1,255 };

	// vertex normals
	bool draw_vertex_normals = false;
	uint vertex_normal_length = 1;
	Color vertex_n_color{ 0,1,1, };
	int vertex_n_width = 1;

	// face normals
	bool draw_face_normals = false;
	uint face_normal_length = 1;
	Color face_n_color{ 1,0,1, };
	int face_n_width = 1;

	// grid
	bool allow_grid = true;

	// outline
	bool outline = false;
	uint parent_line_width = 1;
	uint no_child_line_width = 1;
	Color parent_outline_color{ 1,1,1 };
	Color no_child_outline_color{ 1,1,1 };

	// Bounding Boxes
	bool draw_all_AABB = true;
	Color global_AABB_color{ 1,1,1 };
	uint AABB_line_width = 1;

	// if true, objects with to_delete = true will be deleted
	bool need_to_delete_objects = false;
	

private:

	GameObject* game_object_selected = nullptr;

	std::map<GameObject*, GameObject*> to_reparent;
};