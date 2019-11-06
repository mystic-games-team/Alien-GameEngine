#pragma once
#include "Module.h"
#include "Globals.h"
#include "ModuleObjects.h"

typedef int DragDropFlagsID_;
enum DragDropFlagsID {

	DragDropFlagsID_NONE = 0,
	DragDropFlagsID_HIERARCHY_NODES = 1 << 0,
	DragDropFlagsID_MODEL			= 1 << 1,
	DragDropFlagsID_MESH			= 1 << 2,
	DragDropFlagsID_TEXTURE			= 1 << 3,
	DragDropFlagsID_FOLDER			= 1 << 4,
	IMGUI_PAYLOAD_TYPE_WINDOW		= 1 << 5,
	IMGUI_PAYLOAD_TYPE_COLOR_3F		= 1 << 6,
	IMGUI_PAYLOAD_TYPE_COLOR_4F		= 1 << 7,
	DragDropFlagsID_DEMO			= 1 << 8,

};


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


