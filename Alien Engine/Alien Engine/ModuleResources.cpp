#include "ModuleResources.h"
#include "SDL/include/SDL_assert.h"
#include "Resource_.h"
#include "ResourceMesh.h"
#include "ResourceModel.h"

ModuleResources::ModuleResources(bool start_enabled) : Module(start_enabled)
{
	name = "Resources";
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Start()
{
	ResourceModel* model = new ResourceModel();
	model->ReadMetaData("Library/Models/BakerHouse.alien");
	resource_models.push_back(model);
	resource_models.back()->ConvertToGameObjects();

	return true;
}

update_status ModuleResources::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{

	// TODO: clean up resources

	return true;
}

void ModuleResources::AddResource(Resource* resource)
{
	SDL_assert((uint)ResourceType::RESOURECE_MAX == 2); // add new type

	// TODO: Look if it exists before

	if (resource != nullptr) {
		switch (resource->GetType())
		{
		case ResourceType::RESOURCE_MODEL:
			resource_models.push_back((ResourceModel*)resource);
			break;
		case ResourceType::RESOURCE_MESH:
			resource_meshes.push_back((ResourceMesh*)resource);
			break;
		default:
			break;
		}
	}

}

