#pragma once

class GameObject;

typedef unsigned int uint;

enum class ComponentType {
	TRANSFORM,
	MESH,
	MATERIAL,

	UNKNOWN
};

class Component {
public:

	Component(GameObject* attach);
	virtual ~Component();

	virtual void Enable();
	virtual void Disable();
	virtual bool IsEnabled();

	const ComponentType& GetType() const;


public:

	GameObject* game_object_attached = nullptr;

protected:

	ComponentType type = ComponentType::UNKNOWN;
	bool enabled = true;
	

};
