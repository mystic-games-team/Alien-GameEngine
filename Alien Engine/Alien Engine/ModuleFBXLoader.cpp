#include "ModuleFBXLoader.h"

ModuleImporter::ModuleImporter(bool start_enabled) : Module(start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Start()
{
	return true;
}

update_status ModuleImporter::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	return true;
}
