#pragma once

#include "ModuleImporter.h"
#include "Component.h"
#include "Color.h"

class ResourceTexture;

class __declspec(dllexport) ComponentMaterial : public Component {
	friend class ReturnZ;
	friend class CompZ;
	friend class GameObject;
	friend class ModuleImporter;
	friend class ResourceMesh;
public:
	ComponentMaterial(GameObject* attach);
	virtual ~ComponentMaterial();

private:
	void BindTexture();
	bool DrawInspector();

	void Reset();
	void SetComponent(Component* component);

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	void Clone(Component* clone);

	void SetTexture(ResourceTexture* tex);
	const ResourceTexture* GetTexture() const;

public:
	Color color{ 1,1,1,1 };
	bool texture_activated = true;
private:
	ResourceTexture* texture = nullptr;
	bool change_texture_menu = false;

};