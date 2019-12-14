#pragma once

#include "Globals.h"
#include "Component.h"
#include <vector>
#include <string>
#include "MathGeoLib/include/MathGeoLib.h"
#include "JSONfilepack.h"
#include <map>

enum class ResourceType;
class Resource;

class ComponentCamera;

class __declspec(dllexport) GameObject
{
	friend class Component;
	friend class ComponentCamera;
	friend class ComponentLight;
	friend class ComponentMaterial;
	friend class ComponentTransform;
	friend class ComponentMesh;
	friend class ComponentMaterial;
	friend class ComponentScript;
	friend class GameObject;
	friend class ReturnZ;
	friend class CompZ;
	friend class ModuleCamera3D;
	friend class Octree;
	friend class OctreeNode;
	friend class FileNode;
	friend class ModuleImporter;
	friend class PanelHierarchy;
	friend class PanelScene;
	friend class ModuleRenderer3D;
	friend class PanelInspector;
	friend class ResourceModel;
	friend class ResourceMesh;
	friend class ResourcePrefab;
	friend class ResourceTexture;
	friend class ModuleObjects;
	friend class ModuleUI;
public:
	GameObject(GameObject* parent);
	GameObject(); // just for loading objects, dont use it
	virtual ~GameObject();

public:

	static void Destroy(GameObject* object);
	static GameObject* FindWithName(const char* name);
	static GameObject* FindWithTag(const char* tag_to_find);
	// return the sie of the array of gameobjects found, pass a GameObject** nullptr with &. Remember to delete it!!!
	static uint FindGameObjectsWithTag(const char* tag_to_find, GameObject*** objects);
	// TODO: 
	// static GameObject* Instantiate();

	// TODO: change static, is static... 

		// TODO:
	/*
		GetComponent();
		GetComponentInChildren();
		GetComponentInParent();
		GetComponents();
		GetComponentsInChildren();
		GetComponentsInParent();
		TryGetComponent();

		AddComponent(); ???????????????

		GetInstanceID();

		ToString(); // returns gameobject name

		Destroy();
		DestroyComponent();
		Destroyimmediate();
		DontDestroyOnLoad();

		Function to create prefab or someting xd

	*/

	void SetEnable(bool enable);
	bool IsEnabled();

	// components
	bool HasComponent(ComponentType component);
	void AddComponent(Component* component);
	Component* GetComponent(const ComponentType& type);

	// children
	void AddChild(GameObject* child);
	bool HasChildren();

	// GameObject name
	void SetName(const char* name);
	const char* GetName();

	// GameObject tag
	void SetTag(const char* tag);
	const char* GetTag();
private:
	// OnEnable/Disable
	void OnEnable();
	void OnDisable();

	// here we call Component Mesh, Material & light
	void DrawScene();
	void DrawGame();
	void SetDrawList(std::map<float, GameObject*>* to_draw, const ComponentCamera* camera);

	Component* GetComponentWithID(const u64& ID);
	void RemoveComponent(Component* component);

	template <class Comp>
	Comp* GetComponent();
	template <class Comp>
	std::vector<Comp*> GetComponents();

	// parent
	void SetNewParent(GameObject* new_parent);

	// selected object
	bool IsSelected();
	bool IsParentSelected();
	void ChangeSelected(const bool& select);

	// enabling
	void SayChildrenParentIsEnabled(const bool& enabled);
	bool IsParentEnabled();

	// flip poly if scale negative
	void ScaleNegative(const bool& is_negative);

	// render poly options
	void ChangeWireframe(const bool& wireframe);
	void ChangeMeshView(const bool& wireframe);
	void ChangeVertexNormalsView(const bool& wireframe);
	void ChangeFaceNormalsView(const bool& wireframe);
	void ChangeAABB(const bool& AABB_view);
	void ChangeOBB(const bool& OBB_view);

	// deleting 
	void ToDelete(); // delete it at the begining of the next frame
	void SearchToDelete();

	// search and return true if exists in its children or children of children bla bla
	bool Exists(GameObject* object);

	// Bounding Boxes
	AABB GetBB(); // 0 = Local Bounding Box, 1 = Global Bounding Box
	OBB GetGlobalOBB();
	void SaveObject(JSONArraypack* to_save, const uint& family_number);
	void LoadObject(JSONArraypack* to_save, GameObject* parent);

	void SearchResourceToDelete(const ResourceType& type, Resource* to_delete);

	// prefab
	void SetPrefab(const u64& prefabID);
	void UnpackPrefab();
	void UnpackAllPrefabsOf(const u64& prefabID);
	GameObject* FindPrefabRoot();
	bool IsPrefab() const;
	const u64 GetPrefabID() const;
	void GetObjectWithPrefabID(const u64& prefabID, std::vector<GameObject*>* objs);
	void LockPrefab(bool lock);

	void ResetIDs();

	//static
	void ChangeStatic(bool static_);
	bool HasChildrenStatic() const;

	// find
	GameObject* Find(const char* name);
	GameObject* GetGameObjectByID(const u64& id);
	GameObject* FindTag(const char* tag_to_find);
	void FindTags(const char* tag_to_find, std::vector<GameObject*>* objects);

	// parent selected
	void SayChildrenParentIsSelected(const bool& selected);

	void ReTag(const char* from, const char* to);

public:

	GameObject* parent = nullptr;

private:

	bool to_delete = false; 
	u64 prefabID = 0;

	bool selected = false;

	std::vector<Component*> components;
	std::vector<GameObject*> children;

	char name[MAX_PATH] = "UnNamed";
	char tag[MAX_PATH] = "UnTagged";

	bool enabled = true;
	bool is_static = false;
	u64 ID = 0;
	bool parent_enabled = true;
	bool parent_selected = false;
	bool open_node = false;
	bool prefab_locked = false;
};

template<class Comp>
inline Comp* GameObject::GetComponent()
{
	for (uint i = 0; i < components.size(); ++i) {
		Comp* component = dynamic_cast<Comp*>(components[i]);
		if (component != nullptr) {
			return component;
		}
	}
	return nullptr;
}

template<class Comp>
inline std::vector<Comp*> GameObject::GetComponents()
{
	std::vector<Comp*> comps;
	for (uint i = 0; i < components.size(); ++i) {
		Comp* component = dynamic_cast<Comp*>(components[i]);
		if (component != nullptr) {
			comps.push_back(component);
		}
	}
	return comps;
}
