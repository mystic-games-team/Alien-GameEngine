#include "Resource_.h"

Resource::Resource()
{
}

Resource::~Resource()
{
}

const char* const Resource::GetAssetsPath() const
{
	return path.data();
}

const char* const Resource::GetLibraryPath() const
{
	return meta_data_path.data();
}

const char* const Resource::GetName() const
{
	return name.data();
}

const ResourceType Resource::GetType() const
{
	return type;
}

const u64& Resource::GetID() const
{
	return ID;
}

const bool Resource::NeedToLoad() const
{
	return references == 0;
}

void Resource::IncreaseReferences()
{
	++references;
}

void Resource::DecreaseReferences()
{
	--references;
	if (references == 0)
		FreeMemory();
}
