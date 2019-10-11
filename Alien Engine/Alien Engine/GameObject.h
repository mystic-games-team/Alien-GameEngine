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

public:

	std::vector<Component*> components;
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;

	const char* name = nullptr;

private:

	bool enabled = true;

};