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
	bool ReadMetaData(const char* path);

	// create GameObjects
	void ConvertToGameObjects();

private:

	// sort
	static bool SortByFamilyNumber(const ResourceMesh* mesh1, const ResourceMesh* mesh2);

public:

	std::vector<ResourceMesh*> meshes_attached;

};