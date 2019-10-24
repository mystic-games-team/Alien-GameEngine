#include "ResourceMesh.h"

ResourceMesh::ResourceMesh() : Resource()
{
	type = ResourceType::RESOURCE_MESH;
}

ResourceMesh::~ResourceMesh()
{
}

char* ResourceMesh::CreateMetaData()
{

	return nullptr;
}
