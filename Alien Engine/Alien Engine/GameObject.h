#pragma once

#include "Globals.h"
#include "Component.h"
#include <vector>
#include <string>
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	// OnEnable/Disable
	void OnEnable();
	void OnDisable();
	bool IsEnabled();

	// here we call Component Mesh, Material & light
	void Draw();

	// components
	void AddComponent(Component* component);
	bool CheckComponent(ComponentType component);

	Component* GetComponent(const ComponentType& type);

	// children
	void AddChild(GameObject* child);
	bool HasChildren();

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

	// deleting 
	void ToDelete();
	void SearchToDelete();
private:

	// parent selected
	void SayChildrenParentIsSelected(const bool& selected);

public:

	std::vector<Component*> components;
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;
	bool enabled = true;

private:

	bool to_delete = false;
	bool parent_selected = false;
	bool selected = false;
	bool parent_enabled = true;

	std::string name;
};