#pragma once

#include "Globals.h"
#include "Component.h"
#include <vector>
#include <string>

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	void Enable();
	void Disable();
	bool IsEnabled();
	void Update();
	void AddComponent(Component* component);
	void AddChild(GameObject* child);
	void SetName(const char* name);
	const char* GetName();
	Component* GetComponent(const ComponentType& type);

public:

	std::vector<Component*> components;
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;
	bool enabled = true;
	bool is_selected_in_hierarchy = false;

private:

	
	std::string name;
};