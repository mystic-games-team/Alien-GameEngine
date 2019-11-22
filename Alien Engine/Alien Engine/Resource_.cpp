#include "Resource_.h"
#include "ModuleObjects.h"
#include "Application.h"

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
	this->name = std::string(name);
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
	if (references == 0)
		LoadMemory();
	if (App->objects->enable_instancies) {
		++references;
	}
}

void Resource::DecreaseReferences()
{
	if (App->objects->enable_instancies) {
	--references;
	}
	if (references == 0)
		FreeMemory();
}
