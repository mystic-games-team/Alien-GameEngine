#pragma once

#include "Resource_.h"
#include <vector>

class ResourceMesh;

class ResourceModel : public Resource {

public:

	ResourceModel();
	virtual ~ResourceModel();


public:

	std::vector<ResourceMesh*> meshes;

};