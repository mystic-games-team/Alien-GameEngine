#include "ModuleResources.h"
#include "SDL/include/SDL_assert.h"
#include "Resource_.h"
#include "ResourceMesh.h"
#include "ResourceModel.h"
#include "ModuleImporter.h"
#include "Application.h"
#include "ResourceTexture.h"
#include "RandomHelper.h"
#include "PanelProject.h"
#include "FileNode.h"

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
	icons.jpg_file = App->importer->LoadEngineTexture("Library/Textures/EngineTextures/icon_jpg.png");
	icons.png_file = App->importer->LoadEngineTexture("Library/Textures/EngineTextures/icon_png.png");
	icons.dds_file = App->importer->LoadEngineTexture("Library/Textures/EngineTextures/icon_dds.png");
	icons.folder = App->importer->LoadEngineTexture("Library/Textures/EngineTextures/icon_folder.png");
	icons.model = App->importer->LoadEngineTexture("Library/Textures/EngineTextures/icon_model.png");
	icons.return_icon = App->importer->LoadEngineTexture("Library/Textures/EngineTextures/icon_return.png");
	icons.scene_file = App->importer->LoadEngineTexture("Library/Textures/EngineTextures/icon_scene.png");

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
	std::vector<Resource*>::iterator item = resources.begin();
	for (; item != resources.end(); ++item) {
		if (*item != nullptr) {
			if ((*item)->GetType() == ResourceType::RESOURCE_MODEL)
				static_cast<ResourceModel*>(*item)->meshes_attached.clear();
			delete* item;
			*item = nullptr;
		}
	}
	resources.clear();

	return true;
}

void ModuleResources::AddResource(Resource* resource)
{
	if (resource != nullptr) {
		if (std::find(resources.begin(), resources.end(), resource) == resources.end())
			resources.push_back(resource);
	}
}

bool ModuleResources::CreateNewModelInstanceOf(const char* path)
{
	bool ret = false;
	std::vector<Resource*>::iterator item = resources.begin();
	for (; item != resources.end(); ++item) {
		if (*item != nullptr && (*item)->GetType() == ResourceType::RESOURCE_MODEL) {
			if (App->StringCmp((*item)->GetLibraryPath(), path)) {
				static_cast<ResourceModel*>(*item)->ConvertToGameObjects();
				ret = true;
				break;
			}
		}
	}
	return ret;
}

u64 ModuleResources::GetIDFromAlienPath(const char* path)
{
	u64 ID = 0;
	char* data = nullptr;
	App->file_system->Load(path, &data);

	if (data != nullptr) {

		uint bytes = sizeof(ID);
		memcpy(&ID, data, bytes);

		delete[] data;
	}

	return ID;
}

Resource* ModuleResources::GetResourceWithID(const u64& ID)
{
	std::vector<Resource*>::iterator item = resources.begin();
	for (; item != resources.end(); ++item) {
		if (*item != nullptr && (*item)->GetID() == ID)
			return (*item);
	}
	LOG("No resource found with ID %i", ID);
	return nullptr;
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

	if (App->ui->panel_project != nullptr)
		App->ui->panel_project->current_active_folder = parent;
}

u64 ModuleResources::GetRandomID()
{
	return Random::GetRandomID();
}

ResourceTexture * ModuleResources::GetTextureByName(const char * name)
{
	ResourceTexture* ret = nullptr;

	std::vector<Resource*>::iterator item = resources.begin();
	for (; item != resources.end(); ++item) {
		if (*item != nullptr && (*item)->GetType() == ResourceType::RESOURCE_TEXTURE && App->StringCmp(App->file_system->GetBaseFileName((*item)->GetAssetsPath()).data(),App->file_system->GetBaseFileName(name).data())) {
			return static_cast<ResourceTexture*>(*item);
		}
	}

	return ret;
}

bool ModuleResources::Exists(const char * path, Resource** resource)
{
	bool exists = false;

	std::vector<Resource*>::iterator item = resources.begin();
	for (; item != resources.end(); ++item) {
		if (*item != nullptr && App->StringCmp(App->file_system->GetBaseFileName(path).data(), App->file_system->GetBaseFileName((*item)->GetAssetsPath()).data())) {
			exists = true;
			if (resource != nullptr)
				*resource = (*item);
			break;
		}
	}

	return exists;
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
	SDL_assert((uint)ResourceType::RESOURECE_MAX == 3); // load the new resource

	std::vector<std::string> files;
	std::vector<std::string> directories;

	// Init Textures
	App->file_system->DiscoverFiles(TEXTURES_FOLDER, files, directories);

	ReadTextures(directories, files, TEXTURES_FOLDER);

	files.clear();
	directories.clear();

	// Init Models & Meshes
	App->file_system->DiscoverFiles(MODELS_FOLDER, files, directories);

	ReadModels(directories, files, MODELS_FOLDER);
}

void ModuleResources::ReadTextures(std::vector<std::string> directories, std::vector<std::string> files, std::string current_folder)
{
	for (uint i = 0; i < files.size(); ++i) {
		ResourceTexture* texture = new ResourceTexture();
		texture->LoadMemory(std::string(current_folder + files[i]).data());
	}
	if (!directories.empty()) {
		std::vector<std::string> new_files;
		std::vector<std::string> new_directories;

		for (uint i = 0; i < directories.size(); ++i) {
			std::string dir = current_folder + directories[i] + "/";
			App->file_system->DiscoverFiles(dir.data(), new_files, new_directories);
			ReadTextures(new_directories, new_files, dir);
		}
	}
}

void ModuleResources::ReadModels(std::vector<std::string> directories, std::vector<std::string> files, std::string current_folder)
{
	for (uint i = 0; i < files.size(); ++i) {
		ResourceModel* model = new ResourceModel();
		if (!model->ReadBaseInfo(std::string(current_folder + files[i]).data())) {
			LOG("Error while loading %s because has not .alien", files[i]);
			delete model;
		}
	}
	if (!directories.empty()) {
		std::vector<std::string> new_files;
		std::vector<std::string> new_directories;

		for (uint i = 0; i < directories.size(); ++i) {
			std::string dir = current_folder + directories[i] + "/";
			App->file_system->DiscoverFiles(dir.data(), new_files, new_directories);
			ReadModels(new_directories, new_files, dir);
		}
	}
}


