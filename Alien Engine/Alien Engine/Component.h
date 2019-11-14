#pragma once

class GameObject;

typedef unsigned int uint;
class JSONArraypack;

enum class ComponentType {
	TRANSFORM = 0,
	MESH,
	MATERIAL,
	LIGHT,
	CAMERA,

	UNKNOWN
};

class Component {

public:

	Component(GameObject* attach);
	virtual ~Component();

	virtual void OnEnable() {}
	virtual void OnDisable() {}

	bool IsEnabled();
	void SetEnable(bool enable);
	virtual void DrawInspector() {}

	virtual void Reset() {}
	virtual void SetComponent(Component* component) {}

	virtual void SaveComponent(JSONArraypack* to_save) {}
	virtual void LoadComponent(JSONArraypack* to_load) {}

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
