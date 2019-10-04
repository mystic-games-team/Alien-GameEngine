#pragma once

#include "Globals.h"
#include "Module.h"

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

};