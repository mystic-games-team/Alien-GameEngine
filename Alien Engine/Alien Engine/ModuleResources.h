#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleObjects.h"

#define DROP_ID_HIERARCHY_NODES "hierarchy_node"
#define DROP_ID_PROJECT_NODE "project_node"

struct FileNode;
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

	// meta path
	bool CreateNewModelInstanceOf(const char* path);

	u64 GetIDFromAlienPath(const char* path);
	Resource* GetResourceWithID(const u64& ID);

	void AddNewFileNode(const std::string& path, bool is_file);

	void SetNewMetaName(std::string new_name, std::string meta_user_path, const FileDropType& type);

	u64 GetRandomID();

private:
	FileNode* GetFileNodeByPath(const std::string& path, FileNode* node);
	void ReadAllMetaData();

public:

	Icons icons;

	std::vector<ResourceModel*> resource_models;
	std::vector<ResourceMesh*> resource_meshes;
	std::vector<ResourceTexture*> resource_textures;

	FileNode* assets = nullptr;
};


