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
	// Load Icons
	icons.jpg_file = App->importer->LoadTextureFile("Library/Textures/EngineTextures/icon_jpg.png", false, false);
	icons.png_file = App->importer->LoadTextureFile("Library/Textures/EngineTextures/icon_png.png", false, false);
	icons.dds_file = App->importer->LoadTextureFile("Library/Textures/EngineTextures/icon_dds.png", false, false);
	icons.folder = App->importer->LoadTextureFile("Library/Textures/EngineTextures/icon_folder.png", false, false);
	icons.model = App->importer->LoadTextureFile("Library/Textures/EngineTextures/icon_model.png", false, false);

	assets = new FileNode();
	assets->is_file = false;
	assets->is_base_file = true;
	assets->name = "Assets";

	App->file_system->DiscoverEverythig(assets);
	// TODO: look if all meta data has its fbx or texture if not remove meta data

	ReadAllMetaData();

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
			if (std::find(resource_models.begin(), resource_models.end(), resource) == resource_models.end())
				resource_models.push_back((ResourceModel*)resource);
			break;
		case ResourceType::RESOURCE_MESH:
			if (std::find(resource_meshes.begin(), resource_meshes.end(), resource) == resource_meshes.end())
				resource_meshes.push_back((ResourceMesh*)resource);
			break;
		case ResourceType::RESOURCE_TEXTURE:
			if (std::find(resource_textures.begin(), resource_textures.end(), resource) == resource_textures.end())
				resource_textures.push_back((ResourceTexture*)resource);
			break;
		default:
			break;
		}
	}

}

bool ModuleResources::CreateNewModelInstanceOf(const char* path)
{
	bool ret = false;
	std::vector<ResourceModel*>::iterator item = resource_models.begin();
	for (; item != resource_models.end(); ++item) {
		if (*item != nullptr) {
			if (App->StringCmp((*item)->GetLibraryPath(), path)) {
				(*item)->ConvertToGameObjects();
				ret = true;
				break;
			}
		}
	}
	return ret;
}

void ModuleResources::AddNewFileNode(const std::string& path, bool is_file)
{
	std::string folder = App->file_system->GetCurrentHolePathFolder(path);

	FileNode* parent = GetFileNodeByPath(folder, assets);

	std::string name_file = App->file_system->GetBaseFileNameWithExtension(path.data());

	bool exists = false;
	for (uint i = 0; i < parent->children.size(); ++i) {
		if (parent->children[i] != nullptr && App->StringCmp(parent->children[i]->name.data(), name_file.data())) {
			exists = true;
			break;
		}
	}
	if (!exists)
		parent->children.push_back(new FileNode(name_file, is_file, parent));
}

void ModuleResources::SetNewMetaName(std::string new_name, std::string meta_user_path, const FileDropType& type)
{
	switch (type) {
	case FileDropType::MODEL3D: {
		std::vector<ResourceModel*>::iterator item = resource_models.begin();
		for (; item != resource_models.end(); ++item) {
			if (*item != nullptr && App->StringCmp(meta_user_path.data(), (*item)->GetLibraryPath())) {
				(*item)->ChangeFileMetaName(new_name.data());
				break;
			}
		}
		break; }
	case FileDropType::TEXTURE:
		// TODO
		break;
	}


}

FileNode* ModuleResources::GetFileNodeByPath(const std::string& path, FileNode* node)
{
	FileNode* to_search = nullptr;
	if (App->StringCmp(node->path.data(), path.data())) {
		return node;
	}

	for (uint i = 0; i < node->children.size(); ++i) {
		if (node->children[i] != nullptr) {
			to_search = GetFileNodeByPath(path, node->children[i]);
			if (to_search != nullptr)
				break;
		}
	}
	return to_search;
}

void ModuleResources::ReadAllMetaData()
{
	std::vector<std::string> files;
	std::vector<std::string> directories;

	App->file_system->DiscoverFiles(LIBRARY_MODELS_FOLDER, files, directories);

	for (uint i = 0; i < files.size(); ++i) {
		ResourceModel* model = new ResourceModel();
		model->ReadMetaData(std::string(LIBRARY_MODELS_FOLDER + files[i]).data());
	}

	// TODO: Textures
}


