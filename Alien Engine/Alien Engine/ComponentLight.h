#pragma once

#include "Component.h"
#include "Color.h"
#include "ComponentMesh.h"

class __declspec(dllexport) ComponentLight : public Component {
	friend class GameObject;
	friend class ComponentMesh;
public:
	ComponentLight(GameObject* attach);
	virtual ~ComponentLight();

private:
	void LightLogic();

	bool DrawInspector();
	void OnDisable();

	void Clone(Component* clone);

	void Reset();
	void SetComponent(Component* component);

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	void DrawIconLight();

public:
	Color ambient{ 0.5f, 0.5f, 0.5f, 1.0f };
	Color diffuse{ 0.75f, 0.75f, 0.75f, 1.0f };

private:
	ComponentMesh* bulb = nullptr;
	bool print_icon = true;

	uint light_id = 0;
};
