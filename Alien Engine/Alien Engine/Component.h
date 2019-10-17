#pragma once

class GameObject;

typedef unsigned int uint;

enum class ComponentType {
	TRANSFORM,
	MESH,
	MATERIAL,
	LIGHT,

	UNKNOWN
};

class Component {
public:

	Component(GameObject* attach);
	virtual ~Component();

	virtual void Enable();
	virtual void Disable();
	virtual bool IsEnabled();
	virtual void DrawInspector() {}

	const ComponentType& GetType() const;

public:

	GameObject* game_object_attached = nullptr;

protected:

	ComponentType type = ComponentType::UNKNOWN;
	bool enabled = true;
	

};
