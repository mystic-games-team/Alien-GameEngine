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
#include "ResourcePrefab.h"
#include "FileNode.h"
#include "ResourceScript.h"


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
	icons.jpg_file = App->importer->LoadEngineTexture("Configuration/EngineTextures/icon_jpg.png");
	icons.png_file = App->importer->LoadEngineTexture("Configuration/EngineTextures/icon_png.png");
	icons.dds_file = App->importer->LoadEngineTexture("Configuration/EngineTextures/icon_dds.png");
	icons.tga_file = App->importer->LoadEngineTexture("Configuration/EngineTextures/icon_tga.png");
	icons.folder = App->importer->LoadEngineTexture("Configuration/EngineTextures/icon_folder.png");
	icons.prefab_icon = App->importer->LoadEngineTexture("Configuration/EngineTextures/icon_prefab.png");
	icons.model = App->importer->LoadEngineTexture("Configuration/EngineTextures/icon_model.png");
	icons.return_icon = App->importer->LoadEngineTexture("Configuration/EngineTextures/icon_return.png");
	icons.scene_file = App->importer->LoadEngineTexture("Configuration/EngineTextures/icon_scene.png");
	icons.move_transform = App->importer->LoadEngineTexture("Configuration/EngineTextures/move.png");
	icons.rotate_transform = App->importer->LoadEngineTexture("Configuration/EngineTextures/rotate.png");
	icons.scale_transform = App->importer->LoadEngineTexture("Configuration/EngineTextures/scales.png");
	icons.redo = App->importer->LoadEngineTexture("Configuration/EngineTextures/redo.png");
	icons.undo = App->importer->LoadEngineTexture("Configuration/EngineTextures/undo.png");
	icons.global = App->importer->LoadEngineTexture("Configuration/EngineTextures/global.png");
	icons.local = App->importer->LoadEngineTexture("Configuration/EngineTextures/local.png");
	icons.play = App->importer->LoadEngineTexture("Configuration/EngineTextures/play.png");
	icons.pause = App->importer->LoadEngineTexture("Configuration/EngineTextures/pause.png");
	icons.once = App->importer->LoadEngineTexture("Configuration/EngineTextures/once.png");
	icons.box = App->importer->LoadEngineTexture("Configuration/EngineTextures/box.png");
	icons.prefab = App->importer->LoadEngineTexture("Configuration/EngineTextures/prefab.png");
	icons.prefab_lock = App->importer->LoadEngineTexture("Configuration/EngineTextures/prefab_lock.png");

	
	camera_mesh = App->importer->LoadEngineModels("Configuration/Engine Models/camera.FBX");
	light_mesh = App->importer->LoadEngineModels("Configuration/Engine Models/bulb.fbx");


	// Load Primitives as resource
	cube = new ResourceMesh();
	sphere = new ResourceMesh();
	rock = new ResourceMesh();
	torus = new ResourceMesh();
	dodecahedron = new ResourceMesh();
	icosahedron = new ResourceMesh();
	octahedron = new ResourceMesh();

	assets = new FileNode();
	assets->is_file = false;
	assets->is_base_file = true;
	assets->name = "Assets";


	// TODO: look if all meta data has its fbx or texture if not remove meta data

	ReadAllMetaData();

	App->file_system->DiscoverEverythig(assets);

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

	delete cube;
	delete sphere;
	delete rock;
	delete torus;
	delete dodecahedron;
	delete icosahedron;
	delete octahedron;

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
	
	JSON_Value* value = json_parse_file(path);
	JSON_Object* object = json_value_get_object(value);

	if (value != nullptr && object != nullptr)
	{
		JSONfilepack* meta = new JSONfilepack(path, object, value);

		ID = std::stoull(meta->GetString("Meta.ID"));

		delete meta;
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

ResourceMesh* ModuleResources::GetPrimitive(const PrimitiveType& type)
{	
	ResourceMesh* ret = nullptr;

	switch (type)
	{
	case PrimitiveType::CUBE:
		ret = cube;
		break;
	case PrimitiveType::SPHERE_ALIEN:
		ret = sphere;
		break;
	case PrimitiveType::ROCK:
		ret = rock;
		break;
	case PrimitiveType::DODECAHEDRON:
		ret = dodecahedron;
		break;
	case PrimitiveType::OCTAHEDRON:
		ret = octahedron;
		break;
	case PrimitiveType::TORUS:
		ret = torus;
		break;
	case PrimitiveType::ICOSAHEDRON:
		ret = icosahedron;
		break;
	default:
		break;
	}
	
	if (ret != nullptr) {
		if (ret->NeedToLoad())
			CreatePrimitive(type, &ret);
		if (App->objects->enable_instancies) {
			++ret->references;
		}
	}

	return ret;
}

bool ModuleResources::Exists(const char * path, Resource** resource)
{
	bool exists = false;

	std::vector<Resource*>::iterator item = resources.begin();
	for (; item != resources.end(); ++item) {
		if (*item != nullptr && App->StringCmp(path, (*item)->GetAssetsPath())) {
			exists = true;
			if (resource != nullptr)
				*resource = (*item);
			break;
		}
	}

	return exists;
}

void ModuleResources::CreatePrimitive(const PrimitiveType& type, ResourceMesh** ret)
{
	(*ret)->is_primitive = true;
	par_shapes_mesh* par_mesh = nullptr;

	switch (type)
	{
	case PrimitiveType::CUBE: {
		par_mesh = par_shapes_create_cube();
		(*ret)->SetName("Cube");
		break; }
	case PrimitiveType::SPHERE_ALIEN: {
		par_mesh = par_shapes_create_subdivided_sphere(5);
		(*ret)->SetName("Sphere");
		break; }
	case PrimitiveType::ROCK: {
		par_mesh = par_shapes_create_rock(3, 3);
		(*ret)->SetName("Rock");
		break; }
	case PrimitiveType::DODECAHEDRON: {
		par_mesh = par_shapes_create_dodecahedron();
		(*ret)->SetName("Dodecahedron");
		break; }
	case PrimitiveType::OCTAHEDRON: {
		par_mesh = par_shapes_create_octahedron();
		(*ret)->SetName("Octahedron");
		break; }
	case PrimitiveType::TORUS: {
		par_mesh = par_shapes_create_torus(12, 12, 0.5F);
		(*ret)->SetName("Torus");
		break; }
	case PrimitiveType::ICOSAHEDRON: {
		par_mesh = par_shapes_create_icosahedron();
		(*ret)->SetName("Icosahedron");
		break; }
	default: {
		break; }
	}
	
	App->importer->LoadParShapesMesh(par_mesh, *ret);
}

void ModuleResources::ReadHeaderFile(const char* path, std::vector<std::string> current_scripts)
{
	ResourceScript* script = new ResourceScript();
	script->SetAssetsPath(path);
	script->SetName(App->file_system->GetBaseFileName(path).data());
	for (uint i = 0; i < current_scripts.size(); ++i) {
		if (App->StringCmp(App->file_system->GetBaseFileName(path).data(), App->file_system->GetBaseFileName(current_scripts[i].data()).data())) {
			script->ReadBaseInfo(current_scripts[i].data());
			return;
		}
	}
	// if it goes here it is because it doesnt exist
	script->CreateMetaData();
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

	files.clear();
	directories.clear();

	// Init Prefabs
	App->file_system->DiscoverFiles(ASSETS_PREFAB_FOLDER, files, directories);
	ReadPrefabs(directories, files, ASSETS_PREFAB_FOLDER);

	files.clear();
	directories.clear();

	// Init Scripts
	ReadScripts();
}

void ModuleResources::ReadTextures(std::vector<std::string> directories, std::vector<std::string> files, std::string current_folder)
{
	for (uint i = 0; i < files.size(); ++i) {
		ResourceTexture* texture = new ResourceTexture();
		if (!texture->ReadBaseInfo(std::string(current_folder + files[i]).data())) {
			u64 id = texture->GetID();
			texture->CreateMetaData(id);
		}
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
			App->importer->ReImportModel(model);
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

void ModuleResources::ReadPrefabs(std::vector<std::string> directories, std::vector<std::string> files, std::string current_folder)
{
	for (uint i = 0; i < files.size(); ++i) {
		ResourcePrefab* model = new ResourcePrefab();
		if (!model->ReadBaseInfo(std::string(current_folder + files[i]).data())) {
			LOG("Error while loading %s because has not .alienPrefab", files[i]);
			delete model;
		}
	}
	if (!directories.empty()) {
		std::vector<std::string> new_files;
		std::vector<std::string> new_directories;

		for (uint i = 0; i < directories.size(); ++i) {
			std::string dir = current_folder + directories[i] + "/";
			App->file_system->DiscoverFiles(dir.data(), new_files, new_directories);
			ReadPrefabs(new_directories, new_files, dir);
		}
	}
}

void ModuleResources::ReadScripts()
{
	std::vector<std::string> files;
	std::vector<std::string> directories;
	std::vector<std::string> scripts;

	App->file_system->DiscoverFiles(SCRIPTS_FOLDER, files, directories, true);
	GetAllScriptsPath(directories, files, SCRIPTS_FOLDER, &scripts);

	files.clear();
	directories.clear();

	App->file_system->DiscoverFiles(HEADER_SCRIPTS_FILE, files, directories);

	for (uint i = 0; i < files.size(); ++i) {
		if (files[i].back() == 'h') { // header file found
			ReadHeaderFile(std::string(HEADER_SCRIPTS_FILE + files[i]).data(), scripts);
		}
	}
}

void ModuleResources::GetAllScriptsPath(std::vector<std::string> directories, std::vector<std::string> files, std::string current_folder, std::vector<std::string>* scripts)
{
	if (!files.empty())
		scripts->assign(files.begin(), files.end());
	if (!directories.empty()) {
		std::vector<std::string> new_files;
		std::vector<std::string> new_directories;

		for (uint i = 0; i < directories.size(); ++i) {
			std::string dir = current_folder + directories[i] + "/";
			App->file_system->DiscoverFiles(dir.data(), new_files, new_directories);
			GetAllScriptsPath(new_directories, new_files, dir, scripts);
		}
	}
}



