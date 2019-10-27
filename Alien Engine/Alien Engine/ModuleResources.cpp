#include "ModuleResources.h"
#include "SDL/include/SDL_assert.h"
#include "Resource_.h"
#include "ResourceMesh.h"
#include "ResourceModel.h"
#include "ModuleImporter.h"
#include "Application.h"
#include "ResourceTexture.h"

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

	// Load Icons
	icons.jpg_file = App->importer->LoadTextureFile("Library/Textures/EngineTextures/icon_jpg.png");
	icons.jpg_file->is_custom = false;

	icons.png_file = App->importer->LoadTextureFile("Library/Textures/EngineTextures/icon_png.png");
	icons.png_file->is_custom = false;

	icons.dds_file = App->importer->LoadTextureFile("Library/Textures/EngineTextures/icon_dds.png");
	icons.dds_file->is_custom = false;

	icons.folder = App->importer->LoadTextureFile("Library/Textures/EngineTextures/icon_folder.png");
	icons.folder->is_custom = false;
	
	icons.model = App->importer->LoadTextureFile("Library/Textures/EngineTextures/icon_model.png");
	icons.model->is_custom = false;

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
	SDL_assert((uint)ResourceType::RESOURECE_MAX == 3); // add new type

	if (resource != nullptr) {
		switch (resource->GetType())
		{
		case ResourceType::RESOURCE_MODEL:
			if (std::find(resource_models.begin(), resource_models.end(), resource) == resource_models.begin())
				resource_models.push_back((ResourceModel*)resource);
			break;
		case ResourceType::RESOURCE_MESH:
			if (std::find(resource_meshes.begin(), resource_meshes.end(), resource) == resource_meshes.begin())
				resource_meshes.push_back((ResourceMesh*)resource);
			break;
		case ResourceType::RESOURCE_TEXTURE:
			if (std::find(resource_textures.begin(), resource_textures.end(), resource) == resource_textures.begin())
				resource_textures.push_back((ResourceTexture*)resource);
			break;
		default:
			break;
		}
	}

}

const std::vector<ResourceTexture*> ModuleResources::GetTextures() const
{
	return resource_textures;
}

