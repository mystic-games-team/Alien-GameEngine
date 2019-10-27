#pragma once

#include "ModuleImporter.h"
#include "Component.h"
#include "Color.h"

class ResourceTexture;

class ComponentMaterial : public Component {
public:
	ComponentMaterial(GameObject* attach);
	virtual ~ComponentMaterial();
	
	void BindTexture();
	void DrawInspector();

	void Reset();
	void SetComponent(Component* component);

public:

	ResourceTexture* texture = nullptr;
	uint material_index = 0;
	Color color{ 1,1,1,1 };

private:

	bool change_texture_menu = false;
	bool texture_activated = true;
};