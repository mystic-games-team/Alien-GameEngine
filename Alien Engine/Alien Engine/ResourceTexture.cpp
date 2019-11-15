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
	glDeleteTextures(1, &id);
}

bool ResourceTexture::CreateMetaData()
{
	bool ret = false;
	if (ilLoadImage(path.data())) {
		ID = App->resources->GetRandomID();
		iluFlipImage();
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
		meta_data_path = std::string(LIBRARY_TEXTURES_FOLDER + std::to_string(ID) + ".dds");
		if (App->StringCmp(output.data(), "dds")) {
			App->file_system->Copy(path.data(), meta_data_path.data());
			id = ilutGLBindTexImage();
			is_custom = true;
			width = ilGetInteger(IL_IMAGE_WIDTH);
			height = ilGetInteger(IL_IMAGE_HEIGHT);
		}
		else {
			iluFlipImage();
			ILuint image_size;
			ILubyte* image_data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
			image_size = ilSaveL(IL_DDS, NULL, 0); 
			if (image_size > 0) {
				image_data = new ILubyte[image_size]; 
				if (ilSaveL(IL_DDS, image_data, image_size) > 0) {
					App->file_system->SaveUnique(output, image_data, image_size, LIBRARY_TEXTURES_FOLDER, std::to_string(ID).data(), ".dds");
				}
				iluFlipImage();
				id = ilutGLBindTexImage();
				is_custom = true;
				width = ilGetInteger(IL_IMAGE_WIDTH);
				height = ilGetInteger(IL_IMAGE_HEIGHT);
				
				RELEASE_ARRAY(image_data);
			}
		}

		delete[] data;
		ret = true;
		glBindTexture(GL_TEXTURE_2D, 0);
		App->resources->AddResource(this);
	}
	else {
		delete this;
		ret = false;
	}
	return ret;
}

bool ResourceTexture::ReadMetaData(const char* library_file_path)
{
	bool ret = true;

	this->path = library_file_path;

	std::string alien_path = App->file_system->GetPathWithoutExtension(path) + "_meta.alien";

	char* data = nullptr;

	App->file_system->Load(alien_path.data(), &data);

	if (data != nullptr) {

		memcpy(&ID, data, sizeof(ID));

		delete[] data;
	}

	meta_data_path = LIBRARY_TEXTURES_FOLDER + std::to_string(ID) + ".dds";

	App->importer->LoadTextureToResource(meta_data_path.data(), this);

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

