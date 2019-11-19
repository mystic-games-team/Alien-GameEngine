#pragma once 

#include "ModuleObjects.h"

class Action;
class ActionDeleteObject;
class ObjZ;
class CompZ;

class ReturnZ {

public:

	enum class ReturnActions {
		UNKNOWKN = -1,

		DELETE_OBJECT,
		ADD_OBJECT,
		DELETE_COMPONENT,
		ADD_COMPONENT,
		CHANGE_COMPONENT,
		REPARENT_HIERARCHY
	};

	ReturnZ() {}
	~ReturnZ();

	static void AddNewAction(const ReturnActions& type, void* data, bool user = true);
	static void AddNewFordwarAction(const ReturnActions& type, void* data);

	static void GoBackOneAction();
	static void GoFordwardOneAction();

	static void DoAction(ReturnZ* to_return, bool is_fordward);

public:

	Action* action = nullptr;

private:

	static bool eraseY;

private:

	static void SetDeleteObject(GameObject* obj, ActionDeleteObject* to_fill);
	static void CreateObject(ActionDeleteObject* obj);
	
	void SetAction(const ReturnActions& type, void* data);
};



class Action {
public:
	virtual ~Action() {}
	ReturnZ::ReturnActions type = ReturnZ::ReturnActions::UNKNOWKN;
};

// for deleting objects
class ActionDeleteObject : public Action {
public:
	~ActionDeleteObject();
	ObjZ* object = nullptr;
};

class ActionReparent : public Action {
public:
	u64 objectID = 0;
	u64 parentID = 0;
};

// for creating objects
class ActionAddObject : public Action {
public:
	u64 objectID = 0;
};

// changing components & deleting
class ActionComponent : public Action {
public:
	~ActionComponent();
	CompZ* comp = nullptr;
};

// add 
class ActionAddComponent : public Action {
public:
	u64 compID = 0;
	u64 objectID = 0;
};


class ObjZ {

public:
	~ObjZ();
	bool enabled = false;
	bool is_static = false;
	u64 ID = 0;
	u64 parentID = 0;
	bool to_delete = false;
	bool parent_selected = false;
	bool selected = false;
	bool parent_enabled = false;

	std::string name = "UnNamed";

	std::vector<ActionDeleteObject*> children;

	std::vector<CompZ*> comps;

};

class CompZ {
public:
	virtual ~CompZ() {}
	static void SetCompZ(Component* component, CompZ** compZ);
	static void SetComponent(Component* component, CompZ* compZ);
	static void AttachCompZToGameObject(CompZ* compZ);

	ComponentType type = ComponentType::UNKNOWN;
	u64 objectID = 0;
	u64 compID = 0;
	bool enabled = true;
};

class CompMeshZ : public CompZ{
public:
	u64 resourceID = 0;
	bool view_mesh = false;
	bool wireframe = false;
	bool view_vertex_normals = false;
	bool view_face_normals = false;
	bool draw_AABB = true;
	bool draw_OBB = true;
	bool is_primitive = false;
	PrimitiveType type = PrimitiveType::UNKONWN;
};

class CompMaterialZ : public CompZ {
public:
	u64 resourceID = 0;
	Color color{ 1,1,1,1 };
	bool texture_activated = true;
};

class CompTransformZ : public CompZ {
public:
	float3 pos = float3::zero;
	float3 scale = float3::zero;
	Quat rot = Quat::identity;
};

class CompLightZ : public CompZ {
public:
	Color ambient{ 0.5f, 0.5f, 0.5f, 1.0f };
	Color diffuse{ 0.75f, 0.75f, 0.75f, 1.0f };
};

class CompCameraZ : public CompZ {
public:
	float vertical_fov = 0.f;
	float horizontal_fov = 0.f;
	Color camera_color_background{ 1, 1, 1, 1 };
	float far_plane = 0.f;
	float near_plane = 0.f;
	int is_fov_horizontal = 0;
};