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
class Prefab;
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
	friend class Prefab;
	friend class Octree;
	friend class OctreeNode;
	friend class FileNode;
	friend class ModuleImporter;
	friend class PanelHierarchy;
	friend class PanelScene;
	friend class ModuleRenderer3D;
	friend class PanelCreateObject;
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
	static void DestroyInstantly(GameObject* object);
	static GameObject* FindWithName(const char* name);
	static GameObject* FindWithTag(const char* tag_to_find);
	// return the sie of the array of gameobjects found, pass a GameObject** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	static uint FindGameObjectsWithTag(const char* tag_to_find, GameObject*** objects);
	static void FreeArrayMemory(void*** array_);
	// parent = nullptr is root
	static GameObject* Instantiate(const Prefab& prefab, const float3& position, GameObject* parent = nullptr);
	static GameObject* CloneObject(GameObject* to_clone, GameObject* parent = nullptr);
	// TODO:
	/*
		change static, is static... cant move in code if is static!!

		DontDestroyOnLoad();
	*/



	GameObject* GetChild(const char* child_name);
	GameObject* GetChild(const int& index);
	// look for child of child of child bla bla
	GameObject* GetChildRecursive(const char* child_name);

	void SetEnable(bool enable);
	bool IsEnabled() const;

	// parent
	void SetNewParent(GameObject* new_parent);

	// components
	bool HasComponent(ComponentType component) const;
	Component* GetComponent(const ComponentType& type);
	const Component* GetComponent(const ComponentType& type) const;
	void* GetComponentScript(const char* script_class_name);
	const void* GetComponentScript(const char* script_class_name) const;
	Component* GetComponentInParent(const ComponentType& type);
	const Component* GetComponentInParent(const ComponentType& type) const;
	void* GetComponentScriptInParent(const char* script_class_name);
	const void* GetComponentScriptInParent(const char* script_class_name) const;
	Component* GetComponentInChildren(const ComponentType& type, bool recursive);
	const Component* GetComponentInChildren(const ComponentType& type, bool recursive) const;
	// return the sie of the array of components found, pass a Component** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	uint GetComponents(const ComponentType& type, Component*** comp_array);
	const uint GetComponents(const ComponentType& type, Component*** comp_array) const;
	// return the sie of the array of components found, pass a Component** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	uint GetComponentsInChildren(const ComponentType& type, Component*** comp_array, bool recursive);
	const uint GetComponentsInChildren(const ComponentType& type, Component*** comp_array, bool recursive) const;
	// return the sie of the array of components found, pass a Component** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	uint GetComponentsInParent(const ComponentType& type, Component*** comp_array);
	const uint GetComponentsInParent(const ComponentType& type, Component*** comp_array) const;
	// return the sie of the array of components found, pass a Component** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	uint GetComponentsScript(const char* script_class_name, void*** script_array);
	const uint GetComponentsScript(const char* script_class_name, void*** script_array) const;
	// return the sie of the array of components found, pass a ScriptClassToFind** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	uint GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive);
	const uint GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive) const;
	// return the sie of the array of components found, pass a Component** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	uint GetComponentsScriptInParent(const char* script_class_name, void*** script_array);
	const uint GetComponentsScriptInParent(const char* script_class_name, void*** script_array) const;

	// children
	bool HasChildren() const;

	// GameObject name
	void SetName(const char* name);
	const char* GetName() const;
	const char* ToString();

	// GameObject tag
	void SetTag(const char* tag);
	const char* GetTag() const;

	// delete it at the begining of the next frame
	void ToDelete(); 

private:
	// OnEnable/Disable
	void OnEnable();
	void OnDisable();

	// here we call Component Mesh, Material & light
	void DrawScene();
	void DrawGame();
	void SetDrawList(std::vector<std::pair<float, GameObject*>>* to_draw, const ComponentCamera* camera);

	Component* GetComponentWithID(const u64& ID);
	const Component* GetComponentWithID(const u64& ID) const;
	void RemoveComponent(Component* component);
	void AddComponent(Component* component);

	template <class Comp>
	Comp* GetComponent();
	template <class Comp>
	std::vector<Comp*> GetComponents();

	template <class Comp>
	const Comp* GetComponent() const;
	template <class Comp>
	const std::vector<Comp*> GetComponents() const;

	void GetComponentsChildren(const ComponentType& type, std::vector<Component*>* to_fill, bool recursive);
	void GetComponentsChildren(const ComponentType& type, std::vector<Component*>* to_fill, bool recursive) const;
	void GetComponentsScriptChildren(const char* script_calss_name, std::vector<void*>* to_fill, bool recursive);
	void GetComponentsScriptChildren(const char* script_calss_name, std::vector<void*>* to_fill, bool recursive) const;

	void AddChild(GameObject* child);

	// selected object
	bool IsSelected() const;
	bool IsParentSelected() const;
	void ChangeSelected(const bool& select);

	// enabling
	void SayChildrenParentIsEnabled(const bool& enabled);
	bool IsParentEnabled() const;

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
	void SearchToDelete();

	// search and return true if exists in its children or children of children bla bla
	bool Exists(GameObject* object) const;

	// Bounding Boxes
	AABB GetBB() const; // 0 = Local Bounding Box, 1 = Global Bounding Box
	OBB GetGlobalOBB() const;
	void SaveObject(JSONArraypack* to_save, const uint& family_number);
	void LoadObject(JSONArraypack* to_save, GameObject* parent, bool force_no_selected = false);

	GameObject* Clone(GameObject* parent = nullptr);
	void CloningGameObject(GameObject* clone);

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

template<class Comp>
inline const Comp* GameObject::GetComponent() const
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
inline const std::vector<Comp*> GameObject::GetComponents() const
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
