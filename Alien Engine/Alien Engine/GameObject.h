#pragma once

#include "Globals.h"
#include "Component.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	void Enable();
	void Disable();
	bool IsEnabled();

	void AddComponent(const Component* component);

public:

	std::vector<Component*> components;
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;

	const char* name = nullptr;

private:

	bool enabled = true;

};