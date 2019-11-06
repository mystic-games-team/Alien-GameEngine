#include "ResourceTexture.h"

ResourceTexture::ResourceTexture(const char* path, const uint& id, const uint& width, const uint& height) : Resource()
{
	this->path = std::string(path);
	this->id = id;
	this->width = width;
	this->height = height;

	type = ResourceType::RESOURCE_TEXTURE;
}

ResourceTexture::~ResourceTexture()
{
	// TODO: Clean Up
}

