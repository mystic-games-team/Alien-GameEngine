#include "Resource_.h"

Resource::Resource()
{
}

Resource::~Resource()
{
}

const char* const Resource::GetPath() const
{
	return path.data();
}

const char* const Resource::GetName() const
{
	return name.data();
}
