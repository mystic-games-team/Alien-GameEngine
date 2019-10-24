#pragma once

#include "Resource_.h"
#include <vector>

class ResourceMesh;

class ResourceModel : public Resource {

	friend class ModuleImporter;

public:

	ResourceModel();
	virtual ~ResourceModel();

	// meta data
	void CreateMetaData();
	bool ReadMetaData(char* path);

	// create GameObjects
	void ConvertToGameObjects();

public:

	std::vector<ResourceMesh*> meshes_attached;

	std::string original_path;

};