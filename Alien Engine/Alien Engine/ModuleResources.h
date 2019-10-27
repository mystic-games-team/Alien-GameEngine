#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleObjects.h"

#define DROP_ID_HIERARCHY_NODES "node"
#define DROP_ID_MODEL "model"
#define DROP_ID_MESH "mesh"
#define DROP_ID_TEXTURE "texture"


class Resource;
class ResourceModel;
class ResourceMesh;
class ResourceTexture;

struct Icons {

	ResourceTexture* jpg_file = nullptr;
	ResourceTexture* png_file = nullptr;
	ResourceTexture* dds_file = nullptr;
	ResourceTexture* folder = nullptr;
	ResourceTexture* model = nullptr;

};


class ModuleResources : public Module
{
public:

	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void AddResource(Resource* resource);
	
	const std::vector<ResourceTexture*> GetTextures() const;

public:

	Icons icons;

private:

	std::vector<ResourceModel*> resource_models;
	std::vector<ResourceMesh*> resource_meshes;
	std::vector<ResourceTexture*> resource_textures;

};


