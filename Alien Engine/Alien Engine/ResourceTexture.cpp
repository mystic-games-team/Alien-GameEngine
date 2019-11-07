#include "ResourceTexture.h"
#include "ModuleResources.h"
#include "Application.h"

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

void ResourceTexture::CreateMetaData()
{
	if (ilLoadImage(path.data())) {
		ID = App->resources->GetRandomID();

		std::string alien_path = std::string(App->file_system->GetPathWithoutExtension(path) + "_meta.alien").data();

		uint size = sizeof(uint) + sizeof(ID);

		char* data = new char[size];
		char* cursor = data;

		uint bytes = sizeof(ID);
		memcpy(cursor, &ID, bytes);
		cursor += bytes;

		bytes = sizeof((uint)type);
		memcpy(cursor, &type, bytes);

		std::string output;
		App->file_system->SaveUnique(output, data, size, App->file_system->GetPathWithoutExtension(path).data(), "_meta", ".alien");
		
		App->file_system->SplitFilePath(path.data(), nullptr, nullptr, &output);

		if (App->StringCmp(output.data(), "dds")) {
			App->file_system->Copy(path.data(), std::string(LIBRARY_TEXTURES_FOLDER + std::to_string(ID) + ".dds").data());
		}
		else {
			ILuint image_size;
			ILubyte* image_data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
			image_size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
			if (image_size > 0) {
				image_data = new ILubyte[image_size]; // allocate data buffer
				if (ilSaveL(IL_DDS, image_data, image_size) > 0) {// Save to buffer with the ilSaveIL function
					App->file_system->SaveUnique(output, image_data, image_size, LIBRARY_TEXTURES_FOLDER, std::to_string(ID).data(), ".dds");
					meta_data_path = output;
				}
				RELEASE_ARRAY(image_data);
			}
		}
		App->resources->AddResource(this);
	}
	else {
		delete this;
	}
	
}

bool ResourceTexture::ReadMetaData(const char* path)
{
	bool ret = true;

	meta_data_path = path;

	ID = std::stoull(App->file_system->GetBaseFileName(path));

	std::string texture_path = LIBRARY_TEXTURES_FOLDER + std::to_string(ID) + ".dds";

	App->importer->LoadTextureToResource(texture_path.data(), this);

	//set paths
	meta_data_path = path;

	App->resources->AddResource(this);

	return ret;
}

bool ResourceTexture::DeleteMetaData()
{
	remove(meta_data_path.data());

	std::vector<Resource*>::iterator position = std::find(App->resources->resources.begin(), App->resources->resources.end(), static_cast<Resource*>(this));
	if (position != App->resources->resources.end())
		App->resources->resources.erase(position);

	delete this;

	return true;
}

