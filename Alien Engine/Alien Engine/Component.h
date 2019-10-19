#pragma once

class GameObject;

typedef unsigned int uint;

enum class ComponentType {
	TRANSFORM = 0,
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

	virtual void Reset() {}
	virtual void SetComponent(Component* component) {}

	const ComponentType& GetType() const;

public:

	GameObject* game_object_attached = nullptr;
	bool not_destroy = true;

protected:

	void RightClickMenu(const char* collapsing_header_name);

protected:

	ComponentType type = ComponentType::UNKNOWN;
	bool enabled = true;
	

};
