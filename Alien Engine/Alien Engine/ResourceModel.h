#pragma once

#include "Resource_.h"
#include <vector>

class ResourceMesh;

class ResourceModel : public Resource {

public:

	ResourceModel();
	virtual ~ResourceModel();

	void CreateMetaData();

public:

	std::vector<ResourceMesh*> meshes_attached;

};