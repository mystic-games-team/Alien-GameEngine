#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleObjects.h"

class ModuleResources : public Module
{
public:
	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
};