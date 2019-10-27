#pragma once

#include "Resource_.h"
#include <vector>


class ResourceTexture : public Resource {

public:

	ResourceTexture(const char* path, const uint& id, const uint& width, const uint& height);
	virtual ~ResourceTexture();

public:

	bool is_custom = true;

	uint id = 0;
	uint height = 0;
	uint width = 0;

};