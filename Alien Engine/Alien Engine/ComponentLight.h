#pragma once

#include "Component.h"
#include "Color.h"

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

private:

	Color ambient{ 0.5f, 0.5f, 0.5f, 1.0f };
	Color diffuse{ 0.75f, 0.75f, 0.75f, 1.0f };

	uint light_id=0;
};
