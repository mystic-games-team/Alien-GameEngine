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

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	void SetTexture(ResourceTexture* tex);
	const ResourceTexture* GetTexture() const;

public:

	Color color{ 1,1,1,1 };
	bool texture_activated = true;
private:
	ResourceTexture* texture = nullptr;
	bool change_texture_menu = false;

};