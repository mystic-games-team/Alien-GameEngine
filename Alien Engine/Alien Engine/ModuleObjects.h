#pragma once

#include "Module.h"
#include "Globals.h"
#include "Objects.h"
#include "Primitive.h"

#include <vector>

class ModuleObjects : public Module
{
public:
	ModuleObjects(bool start_enabled = true);
	virtual ~ModuleObjects();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	Primitive* CreatePrimitive(const PrimitiveType &type, const float &position_x, const float &position_y, const float &position_z);

	void DrawPrimitive();

public:
	std::vector<Object*> objects;
};