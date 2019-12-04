#pragma once

#include <string>

typedef unsigned int uint;
typedef unsigned long long u64;

enum class ResourceType {
	RESOURCE_NONE = -1,
	RESOURCE_MESH,
	RESOURCE_MODEL,
	RESOURCE_TEXTURE,
	RESOURCE_SCRIPT,

	RESOURECE_MAX
};


class Resource {

public:

	Resource();
	virtual ~Resource();

	// meta data path
	const char* const GetAssetsPath() const;
	const char* const GetLibraryPath() const;
	void SetAssetsPath(const char* path);

	// name
	const char* const GetName() const;
	void SetName(const char* name);
	// type
	const ResourceType GetType() const;

	virtual bool ReadBaseInfo(const char* assets_file_path) { return true; }

	virtual bool CreateMetaData(const u64& force_id = 0) { return true; } // when someting is dropped
	virtual bool DeleteMetaData() { return true; } // this deletes the resource too
	
	virtual bool LoadMemory() { return true; }
	virtual void FreeMemory() {}

	const u64& GetID() const;

	const bool NeedToLoad() const;
	void IncreaseReferences();
	void DecreaseReferences();

public:

	uint references = 0u;

protected:

	ResourceType type = ResourceType::RESOURCE_NONE;

	std::string path;
	std::string meta_data_path;
	std::string name;

	u64 ID = 0;

};