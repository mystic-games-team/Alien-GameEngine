#pragma once

#include <string>

typedef unsigned int uint;

enum class ResourceType {
	RESOURCE_NONE = -1,
	RESOURCE_MESH,
	RESOURCE_MODEL,

	RESOURECE_MAX
};


class Resource {

public:

	Resource();
	virtual ~Resource();

	// path
	const char* const GetPath() const;

	// name
	const char* const GetName() const;

	// type
	const ResourceType GetType() const;

	virtual void CreateMetaData() {}
	virtual void ReadMetaData() {}

protected:

	ResourceType type = ResourceType::RESOURCE_NONE;

	std::string path;
	std::string name;

};