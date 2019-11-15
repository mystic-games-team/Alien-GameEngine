#pragma once

#include "Globals.h"
#include "Component.h"
#include <vector>
#include <string>
#include "MathGeoLib/include/MathGeoLib.h"
#include "JSONfilepack.h"

enum class ResourceType;
class Resource;

class GameObject
{
public:
	GameObject(GameObject* parent);
	GameObject(); // just for loading objects, dont use it
	virtual ~GameObject();

	// OnEnable/Disable
	void OnEnable();
	void OnDisable();
	bool IsEnabled();

	// here we call Component Mesh, Material & light
	void DrawScene();
	void DrawGame();

	// components
	void AddComponent(Component* component);
	bool HasComponent(ComponentType component);
	Component* GetComponent(const ComponentType& type);
	Component* GetComponentWithID(const u64& ID);

	// children
	void AddChild(GameObject* child);
	bool HasChildren();

	// parent
	void SetNewParent(GameObject* new_parent);

	// GameObject name
	void SetName(const char* name);
	const char* GetName();

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

	// id's
	GameObject* GetGameObjectByID(const u64 & id);

	// search and return true if exists in its children or children of children bla bla
	bool Exists(GameObject* object);

	// Bounding Boxes
	AABB GetBB(); // 0 = Local Bounding Box, 1 = Global Bounding Box
	OBB GetGlobalOBB();
	void SaveObject(JSONArraypack* to_save, const uint& family_number);
	void LoadObject(JSONArraypack* to_save, GameObject* parent);

	void SearchResourceToDelete(const ResourceType& type, Resource* to_delete);

	//static
	void ChangeStatic(bool static_);
	bool HasChildrenStatic() const;

private:

	// parent selected
	void SayChildrenParentIsSelected(const bool& selected);

public:

	std::vector<Component*> components;
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;
	bool enabled = true;
	bool is_static = false;
	u64 ID = 0;
	bool parent_enabled = true;
	bool parent_selected = false;
private:

	bool to_delete = false; 

	bool selected = false;


	std::string name = "UnNamed";
};