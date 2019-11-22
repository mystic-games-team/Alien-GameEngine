#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleObjects.h"

#define DROP_ID_HIERARCHY_NODES "hierarchy_node"
#define DROP_ID_PROJECT_NODE "project_node"

class FileNode;
enum class FileDropType;

class Resource;
class ResourceModel;
class ResourceMesh;
class ResourceTexture;

struct Icons {

	ResourceTexture* jpg_file = nullptr;
	ResourceTexture* png_file = nullptr;
	ResourceTexture* dds_file = nullptr;
	ResourceTexture* folder = nullptr;
	ResourceTexture* prefab_icon = nullptr;
	ResourceTexture* model = nullptr;
	ResourceTexture* return_icon = nullptr;
	ResourceTexture* scene_file = nullptr;
	ResourceTexture* move_transform = nullptr;
	ResourceTexture* rotate_transform = nullptr;
	ResourceTexture* scale_transform = nullptr;
	ResourceTexture* undo = nullptr;
	ResourceTexture* redo = nullptr;
	ResourceTexture* global = nullptr;
	ResourceTexture* local = nullptr;
	ResourceTexture* play = nullptr;
	ResourceTexture* pause = nullptr;
	ResourceTexture* once = nullptr;
	ResourceTexture* box = nullptr;
	ResourceTexture* prefab = nullptr;
	ResourceTexture* prefab_lock = nullptr;
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

	// meta path
	bool CreateNewModelInstanceOf(const char* path);

	u64 GetIDFromAlienPath(const char* path);
	Resource* GetResourceWithID(const u64& ID);

	void AddNewFileNode(const std::string& path, bool is_file);

	u64 GetRandomID();

	ResourceTexture* GetTextureByName(const char* name);
	ResourceMesh* GetPrimitive(const PrimitiveType& type);
	bool Exists(const char* path, Resource** resource);

	void CreatePrimitive(const PrimitiveType& type, ResourceMesh** mesh);
private:
	FileNode* GetFileNodeByPath(const std::string& path, FileNode* node);
	void ReadAllMetaData();
	void ReadTextures(std::vector<std::string> directories, std::vector<std::string> files, std::string current_folder);
	void ReadModels(std::vector<std::string> directories, std::vector<std::string> files, std::string current_folder);

	void ReadPrefabs(std::vector<std::string> directories, std::vector<std::string> files, std::string current_folder);

public:

	Icons icons;

	std::vector<Resource*> resources;

	ResourceMesh* camera_mesh = nullptr;
	ResourceMesh* light_mesh = nullptr; 
	FileNode* assets = nullptr;

private:
	ResourceMesh* cube = nullptr;
	ResourceMesh* sphere = nullptr;
	ResourceMesh* rock = nullptr;
	ResourceMesh* torus = nullptr;
	ResourceMesh* dodecahedron = nullptr;
	ResourceMesh* icosahedron = nullptr;
	ResourceMesh* octahedron = nullptr;
};


