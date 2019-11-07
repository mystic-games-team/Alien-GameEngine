#pragma once

#include "Resource_.h"
#include <vector>


class ResourceTexture : public Resource {

public:

	ResourceTexture() { type = ResourceType::RESOURCE_TEXTURE; }
	ResourceTexture(const char* path, const uint& id, const uint& width, const uint& height);
	ResourceTexture(const char* path) { this->path = std::string(path); type = ResourceType::RESOURCE_TEXTURE; }
	virtual ~ResourceTexture();

	void CreateMetaData();
	bool ReadMetaData(const char* path);
	bool DeleteMetaData();

public:

	bool is_custom = true;

	uint id = 0;
	uint height = 0;
	uint width = 0;

};