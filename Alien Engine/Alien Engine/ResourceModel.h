#pragma once

#include "Resource_.h"
#include <vector>

class ResourceMesh;

class ResourceModel : public Resource {

	friend class ModuleImporter;

public:

	ResourceModel();
	virtual ~ResourceModel();

	void CreateMetaData();
	void ReadMetaData();

public:

	std::vector<ResourceMesh*> meshes_attached;

	std::string original_path;

};