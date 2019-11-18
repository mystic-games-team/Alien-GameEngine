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

void Resource::SetName(const char* name)
{
	name = std::string(name).data();
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
	if (ID == 0)
		return;

	if (references == 0)
		LoadMemory();
	++references;
}

void Resource::DecreaseReferences()
{
	if (ID == 0)
		return;

	--references;
	if (references == 0)
		FreeMemory();
}
