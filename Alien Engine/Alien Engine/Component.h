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

	virtual void OnEnable() {}
	virtual void OnDisable() {}

	virtual bool IsEnabled();
	virtual void DrawInspector() {}

	const ComponentType& GetType() const;

public:

	GameObject* game_object_attached = nullptr;
	bool not_destroy = true;

protected:

	ComponentType type = ComponentType::UNKNOWN;
	bool enabled = true;
	

};
