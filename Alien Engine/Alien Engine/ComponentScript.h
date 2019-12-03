#pragma once

#include "Component.h"

class ComponentScript : public Component {
public:
	ComponentScript(GameObject* attach);
	virtual ~ComponentScript();

	void Reset();
	void SetComponent(Component* component);

	bool DrawInspector();

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);
};
