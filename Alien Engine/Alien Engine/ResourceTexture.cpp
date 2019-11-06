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
				if (ilSaveL(IL_DDS, image_data, image_size) > 0) // Save to buffer with the ilSaveIL function
					App->file_system->SaveUnique(output, image_data, image_size, LIBRARY_TEXTURES_FOLDER, std::to_string(ID).data(), ".dds");
				RELEASE_ARRAY(image_data);
			}
		}
		App->resources->AddResource(this);
	}
	else {
		delete this;
	}
	


	//ILuint new_image_id = 0;
	//ilGenImages(1, &new_image_id);
	//ilBindImage(new_image_id);

	//ilutRenderer(ILUT_OPENGL);

	//if (ilLoadImage(path)) {
	//	iluFlipImage();
	//	texture = new ResourceTexture(path, ilutGLBindTexImage(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
	//	texture->is_custom = is_custom;
	//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//	glBindTexture(GL_TEXTURE_2D, texture->id);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	//	App->resources->AddResource(texture);

	//	if (has_been_dropped && App->objects->GetSelectedObject() != nullptr) {
	//		ApplyTextureToSelectedObject(texture);
	//	}

	//	if (is_custom)
	//		App->resources->AddNewFileNode(path, true);
	//	iluFlipImage();
	//	ilSave(IL_DDS, std::string(LIBRARY_TEXTURES_FOLDER + App->file_system->GetBaseFileName(path) + ".dds").data());

	//	LOG("Texture successfully loaded: %s", path);
	//}
	//else {
	//	LOG("Error while loading image in %s", path);
	//	LOG("Error: %s", ilGetString(ilGetError()));
	//}

	//ilDeleteImages(1, &new_image_id);
}

