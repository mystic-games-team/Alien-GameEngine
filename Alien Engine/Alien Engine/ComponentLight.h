#pragma once

#include "Component.h"
#include "Color.h"
#include "ComponentMesh.h"

class ComponentLight : public Component {
public:
	ComponentLight(GameObject* attach);
	virtual ~ComponentLight();

	void LightLogic();

	void DrawInspector();
	void OnDisable();

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
	uint light_id=0;
};
