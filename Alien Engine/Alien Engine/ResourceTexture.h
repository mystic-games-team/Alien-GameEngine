#pragma once

#include "Resource_.h"
#include <vector>


class ResourceTexture : public Resource {

public:

	ResourceTexture() { type = ResourceType::RESOURCE_TEXTURE; }
	ResourceTexture(const char* path, const uint& id, const uint& width, const uint& height);
	ResourceTexture(const char* path);
	virtual ~ResourceTexture();

	bool CreateMetaData(const u64& force_id = 0);
	bool LoadMemory();
	void FreeMemory();
	bool ReadBaseInfo(const char* assets_path);
	void ReadLibrary(const char* meta_data);
	bool DeleteMetaData();

public:

	bool is_custom = true;

	uint height = 0;
	uint width = 0;
	uint id = 0;

};