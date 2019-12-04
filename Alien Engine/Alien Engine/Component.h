#pragma once

class GameObject;

typedef unsigned int uint;
class JSONArraypack;
typedef unsigned long long u64;
enum class ComponentType {
	TRANSFORM = 0,
	MESH,
	MATERIAL,
	LIGHT,
	CAMERA,
	SCRIPT,

	UNKNOWN
};

class __declspec(dllexport) Component {

public:

	Component(GameObject* attach);
	virtual ~Component();

	virtual void OnEnable() {}
	virtual void OnDisable() {}

	bool IsEnabled();
	void SetEnable(bool enable);
	virtual bool DrawInspector() { return true; }

	virtual void PreUpdate() {}
	virtual void Update() {}
	virtual void PostUpdate() {}

	virtual void Reset() {}
	virtual void SetComponent(Component* component) {}

	virtual void SaveComponent(JSONArraypack* to_save) {}
	virtual void LoadComponent(JSONArraypack* to_load) {}

	void ResetIDs();

	const ComponentType& GetType() const;

public:

	u64 ID = 0;
	GameObject* game_object_attached = nullptr;
	bool not_destroy = true;

protected:

	void RightClickMenu(const char* collapsing_header_name);

protected:

	ComponentType type = ComponentType::UNKNOWN;
	bool enabled = true;
	

};
