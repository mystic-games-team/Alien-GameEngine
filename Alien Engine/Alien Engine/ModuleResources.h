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
	ResourceTexture* model = nullptr;
	ResourceTexture* return_icon = nullptr;

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

private:
	FileNode* GetFileNodeByPath(const std::string& path, FileNode* node);
	void ReadAllMetaData();
	void ReadTextures(std::vector<std::string> directories, std::vector<std::string> files, std::string current_folder);

public:

	Icons icons;

	std::vector<Resource*> resources;

	FileNode* assets = nullptr;
};


