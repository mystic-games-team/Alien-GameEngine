#pragma once

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Color.h"
#include <vector>
#include <map>
#include <utility>
#include "Octree.h"
#include "ComponentCamera.h"
#include <stack>
#include <functional>

class ReturnZ;
class ResourcePrefab;
class ComponentScript;
class Alien;
class ResourceScene;

struct InvokeInfo {
	std::function<void()> function = nullptr;
	float time_to_wait = 0.0F;
	float time_started = 0.0F;
	bool is_repeating = false;
	float time_between = 0.0F;
	Alien* alien = nullptr;
	u64 ID = 0;

	bool operator==(const InvokeInfo& info) {
		return ID == info.ID;
	}
};

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
	void DrawRay();
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
	void ChangeEnableOctree();
	void ChangeEnableOBB();
	
	// enable/disable grid
	void ChangeEnableGrid();

	void ChangeEnableOutline();

	void DeleteAllObjects();

	// select/disselect objects
	void SetNewSelectedObject(GameObject* selected);
	const std::list<GameObject*>& GetSelectedObjects();
	void DeselectObjects();
	void DeselectObject(GameObject* obj);

	/*---------Scripts Calls-----------*/
	void InitScriptsOnPlay() const;
	void ScriptsPreUpdate() const;
	void ScriptsUpdate() const;
	void ScriptsPostUpdate() const;
	void CleanUpScriptsOnStop() const;
	void OnDrawGizmos() const;
	void OnPreCull(ComponentCamera* camera) const;
	void OnPreRender(ComponentCamera* camera) const;
	void OnPostRender(ComponentCamera* camera) const;
	/*---------Scripts Calls-----------*/

	// if parent is nullptr, parent will be the invisible game object
	GameObject* CreateEmptyGameObject(GameObject* parent, bool set_selected = true);

	// right click menu, move
	void MoveObjectDown(GameObject* object, bool bottom);
	void MoveObjectUp(GameObject* object, bool top);
	void MoveComponentDown(GameObject* object, Component* component, bool bottom);
	void MoveComponentUp(GameObject* object, Component* component, bool top);
	
	// get object
	GameObject* GetGameObjectByID(const u64& id);

	//reparent object in the next preupdate
	void ReparentGameObject(GameObject* object, GameObject* next_parent, bool to_cntrlZ = true);

	// scenes
	void SaveScene(ResourceScene* scene, const char* force_with_path = nullptr);
	void LoadScene(const char * name, bool change_scene = true);
	void CreateEmptyScene(ResourceScene* scene);

	static bool SortByFamilyNumber(std::tuple<uint, u64, uint> pair1, std::tuple<uint, u64, uint> pair2);
	void SaveGameObject(GameObject* obj, JSONArraypack* to_save, const uint& family_number);

	GameObject* GetRoot(bool ignore_prefab);
	void CreateRoot();

	void SwapReturnZ(bool get_save, bool delete_current);
	
	void HotReload();

	static bool SortGameObjectToDraw(std::pair<float, GameObject*> first, std::pair<float, GameObject*> last);

	void AddScriptObject(const u64& ID, GameObject** object);

	void DuplicateObjects();

	void AddInvoke(std::function<void()> void_no_params_function, const float& second, Alien* alien);
	void AddInvokeRepeating(std::function<void()> void_no_params_function, const float& second, const float& seconds_between_each_call, Alien* alien);
	void CancelInvokes(Alien* alien);
	/*bool IsInvoking(std::function<void()> void_no_params_function);*/

private:

	void CreateJsonScript(GameObject* obj, JSONArraypack* to_save);
	void ReAssignScripts(JSONArraypack* to_load);
	void DeleteReturns();

public:

	ResourceScene* current_scene = nullptr;

	std::list<Alien*> current_scripts;

	Component* component_in_copy = nullptr;

	bool prefab_scene = false;
	bool printing_scene = true;
	// Prefab Scene
	Color prefab_color_background{ 0.2f, 0.4f, 0.6f, 1.0f };

	// Frustum
	bool draw_frustum = true;
	Color frustum_color{ 1,0,0,1 };
	int frustum_line_width = 1;
	bool check_culling_in_scene = false;

	// wireframe
	bool wireframe_mode = false;

	// mesh
	bool view_mesh_mode = true;
	int mesh_line_width = 1;
	Color mesh_color{ 1,1,1,1 };

	// vertex normals
	bool draw_vertex_normals = false;
	uint vertex_normal_length = 1;
	Color vertex_n_color{ 0,1,1, 1};
	int vertex_n_width = 1;

	// face normals
	bool draw_face_normals = false;
	uint face_normal_length = 1;
	Color face_n_color{ 1,0,1, 1};
	int face_n_width = 1;

	// grid
	bool allow_grid = true;

	// octree
	bool render_octree = false;
	uint octree_line_width = 1;
	Color octree_line_color = { 1,0,0,1 };

	// outline
	bool outline = false;
	uint parent_line_width = 1;
	uint no_child_line_width = 1;
	Color parent_outline_color{ 1,1,1 };
	Color no_child_outline_color{ 1,1,1 };

	// Bounding Boxes
	bool draw_all_AABB = false;
	bool draw_all_OBB = false;
	Color global_AABB_color{ 1,1,1 };
	Color global_OBB_color{ 1,1,1 };
	uint AABB_line_width = 1;
	uint OBB_line_width = 1;

	// Ray Options
	bool draw_ray = false;
	Color ray_color{ 1,0,0,1 };
	uint ray_width = 5;

	// if true, objects with to_delete = true will be deleted
	bool need_to_delete_objects = false;
	
	bool errors = false;

	Octree octree;
	std::stack<ReturnZ*> return_actions;
	std::stack<ReturnZ*> fordward_actions;

	bool in_cntrl_Z = false;

	std::vector<ComponentCamera*> game_cameras;
	bool enable_instancies = true;
	ResourcePrefab* prefab_opened = nullptr;
	bool ignore_cntrlZ = false;

	ComponentScript* actual_script_loading = nullptr;

	std::vector<std::string> tags;

private:
	// root
	GameObject* base_game_object = nullptr;
	std::list<GameObject*> game_objects_selected;
	std::vector< std::tuple<GameObject*, GameObject*, bool>> to_reparent;

	std::stack<ReturnZ*> save_return_actions;
	std::stack<ReturnZ*> save_fordward_actions;

	std::vector<std::pair<u64, GameObject**>> to_add;

	std::list<InvokeInfo*> invokes;
};

