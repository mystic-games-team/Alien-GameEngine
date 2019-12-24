#include "ResourceTexture.h"
#include "ModuleResources.h"
#include "Application.h"

ResourceTexture::ResourceTexture(const char* path, const uint& id, const uint& width, const uint& height) : Resource()
{
	this->path = std::string(path);
	this->id = id;
	this->width = width;
	this->height = height;
	name = App->file_system->GetBaseFileName(path);
	type = ResourceType::RESOURCE_TEXTURE;
}

ResourceTexture::ResourceTexture(const char* path)
{
	name = App->file_system->GetBaseFileName(path); 
	this->path = std::string(path); 
	type = ResourceType::RESOURCE_TEXTURE;
}

ResourceTexture::~ResourceTexture()
{
	glDeleteTextures(1, &id);
}

bool ResourceTexture::CreateMetaData(const u64& force_id)
{
	bool ret = false;
	if (ilLoadImage(path.data())) {
		if (force_id == 0)
			ID = App->resources->GetRandomID();
		else
			ID = force_id;

		iluFlipImage();
		std::string alien_path = std::string(App->file_system->GetPathWithoutExtension(path) + "_meta.alien").data();

		JSON_Value* alien_value = json_value_init_object();
		JSON_Object* alien_object = json_value_get_object(alien_value);
		json_serialize_to_file_pretty(alien_value, alien_path.data());

		if (alien_value != nullptr && alien_object != nullptr) {
			JSONfilepack* alien = new JSONfilepack(alien_path, alien_object, alien_value);
			alien->StartSave();
			alien->SetString("Meta.ID", std::to_string(ID));
			alien->FinishSave();
			delete alien;
		}

		meta_data_path = std::string(LIBRARY_TEXTURES_FOLDER + std::to_string(ID) + ".dds");
		std::string ext;
		App->file_system->SplitFilePath(path.data(), nullptr, nullptr, &ext);
		if (App->StringCmp(ext.data(), "dds")) {
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
					std::string output;
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

		ret = true;
		glBindTexture(GL_TEXTURE_2D, 0);
		App->resources->AddResource(this);
		FreeMemory();
	}
	else {
		delete this;
		ret = false;
	}



	return ret;
}

bool ResourceTexture::LoadMemory()
{
	bool ret = true;

	App->importer->LoadTextureToResource(meta_data_path.data(), this);

	return ret;
}

void ResourceTexture::FreeMemory()
{
	glDeleteTextures(1, &id);
	width = 0;
	height = 0;
	id = 0;
}

bool ResourceTexture::ReadBaseInfo(const char* assets_path)
{
	bool ret = true;

	this->path = assets_path;
	std::string alien_path = App->file_system->GetPathWithoutExtension(path) + "_meta.alien";

	JSON_Value* value = json_parse_file(alien_path.data());
	JSON_Object* object = json_value_get_object(value);

	if (value != nullptr && object != nullptr)
	{
		JSONfilepack* meta = new JSONfilepack(alien_path, object, value);

		ID = std::stoull(meta->GetString("Meta.ID"));

		delete meta;
	}

	meta_data_path = LIBRARY_TEXTURES_FOLDER + std::to_string(ID) + ".dds";

	if (!App->file_system->Exists(meta_data_path.data())) {
		return false;
	}

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


