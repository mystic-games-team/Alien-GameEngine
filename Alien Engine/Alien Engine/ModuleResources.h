#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleObjects.h"

class Resource;
class ResourceModel;
class ResourceMesh;

class ModuleResources : public Module
{
public:
	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void AddResource(Resource* resource);

private:

	std::vector<ResourceModel*> resource_models;
	std::vector<ResourceMesh*> resource_meshes;

};