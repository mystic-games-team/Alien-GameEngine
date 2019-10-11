#pragma once

#include "Globals.h"
#include "Component.h"
#include <vector>

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

public:

	std::vector<Component*> components;
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;

	

private:

	bool enabled = true;
	const char* name = nullptr;
};