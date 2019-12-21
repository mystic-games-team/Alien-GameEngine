#include "FileNode.h"
#include "Application.h"
#include "ResourceModel.h"
#include "ResourceTexture.h"
#include "ResourcePrefab.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include "Prefab.h"
FileNode::FileNode()
{
}

FileNode::FileNode(std::string name, bool is_file, FileNode* parent)
{
	this->name = name;
	this->is_file = is_file;
	this->parent = parent;

	std::string previous_names;
	App->file_system->GetPreviousNames(previous_names, parent);
	path = previous_names;

	// set icon
	SetIcon();
}

FileNode::FileNode(const std::string& path, const std::string& name, bool is_file, FileNode* parent)
{
	this->name = name;
	this->is_file = is_file;
	this->parent = parent;
	this->path = path;

	// set icon
	SetIcon();
}

FileNode::~FileNode()
{
	DeleteChildren();
}

void FileNode::DeleteChildren()
{
	std::vector<FileNode*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->DeleteChildren();
			delete (*item);
			*item = nullptr;
		}
	}
	children.clear();
}

void FileNode::DeleteNodeData(bool delete_folder)
{
	if (is_file) {
		std::string hole_path = std::string(path + name).data();

		std::string meta_path;
		if (type == FileDropType::PREFAB) {
			meta_path = App->file_system->GetPathWithoutExtension(hole_path.data()) + ".alienPrefab";
		}
		else {
			meta_path = App->file_system->GetPathWithoutExtension(hole_path.data()) + "_meta.alien";
		}
		u64 ID = App->resources->GetIDFromAlienPath(meta_path.data());
		if (ID != 0) {
			remove(meta_path.data());

			Resource* resource_to_delete = App->resources->GetResourceWithID(ID);
			if (resource_to_delete != nullptr) {
				resource_to_delete->DeleteMetaData();
				auto item = App->resources->resources.begin();
				for (; item != App->resources->resources.end(); ++item) {
					if (*item == resource_to_delete) {
						delete* item;
						*item = nullptr;
						App->resources->resources.erase(item);
						break;
					}
				}
			}
		}
		remove(hole_path.data());
	}
	else {
		std::vector<FileNode*>::iterator item = children.begin();
		for (; item != children.end(); ++item) {
			if (*item != nullptr)
				(*item)->DeleteNodeData(false);
		}

		if (delete_folder) {			
			if (std::experimental::filesystem::remove_all(path.data())) {
				LOG("Folder removed successfully %s", path.data());
			}
			else {
				LOG("Could not remove folder %s", path.data());
			}
		}
	}
}

void FileNode::ResetPaths()
{
	switch (type)
	{
	case FileDropType::MODEL3D: {
		std::string path = App->file_system->GetPathWithoutExtension(this->path + name);
		path += "_meta.alien";
		u64 ID = App->resources->GetIDFromAlienPath(path.data());
		ResourceModel* model = (ResourceModel*)App->resources->GetResourceWithID(ID);
		if (model != nullptr) {
			model->SetAssetsPath(std::string(this->path + name).data());
		}
		break; }
	case FileDropType::TEXTURE: {
		std::string path = App->file_system->GetPathWithoutExtension(this->path + name);
		path += "_meta.alien";
		u64 ID = App->resources->GetIDFromAlienPath(path.data());
		ResourceTexture* texture = (ResourceTexture*)App->resources->GetResourceWithID(ID);
		if (texture != nullptr) {
			texture->SetAssetsPath(std::string(this->path + name).data());
		}
		break; }
	case FileDropType::PREFAB: {
		std::string path = App->file_system->GetPathWithoutExtension(this->path + name);
		path += ".alienPrefab";
		u64 ID = App->resources->GetIDFromAlienPath(path.data());
		ResourcePrefab* prefab = (ResourcePrefab*)App->resources->GetResourceWithID(ID);
		if (prefab != nullptr) {
			prefab->SetAssetsPath(std::string(this->path + name).data());
			prefab->SetLibraryPath(std::string(this->path + name).data());
			prefab->SetName(App->file_system->GetBaseFileName(path.data()).data());
			auto item = prefab->prefab_references.begin();
			for (; item != prefab->prefab_references.end(); ++item) {
				if (*item != nullptr) {
					(*item)->prefab_name = std::string(prefab->name);
				}
			}
		}
		break; }

	default: {
		LOG("Type in reset paths not added");
		break; }
	}
}

FileNode* FileNode::FindChildrenByPath(const std::string& path)
{
	FileNode* ret = nullptr;
	std::vector<FileNode*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (ret != nullptr)
			break;
		if (*item != nullptr) {
			if (App->StringCmp((*item)->path.data(), path.data()))
				return (*item);
			else
				ret = (*item)->FindChildrenByPath(path);
		}
	}
	return ret;
}

void FileNode::RemoveResourceOfGameObjects()
{
	if (is_file) {
		SDL_assert((uint)FileDropType::UNKNOWN == 5);
		switch (type) {
		case FileDropType::SCENE:
			static char curr_dir[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, curr_dir);
			if (App->StringCmp(App->objects->current_scene.full_path.data(), std::string(curr_dir + std::string("/") + path + name).data())) {
				App->objects->CreateRoot();
				App->objects->current_scene.name_without_extension = "Untitled*";
				App->objects->current_scene.full_path = "Untitled*";
				App->objects->current_scene.need_to_save = false;
				App->objects->current_scene.is_untitled = true;
			}
			break;
		case FileDropType::SCRIPT:
			// TODO:
			break;
		case FileDropType::TEXTURE: {
			std::string path_ = App->file_system->GetPathWithoutExtension(path + name);
			path_ += "_meta.alien";
			u64 ID = App->resources->GetIDFromAlienPath(path_.data());
			ResourceTexture* texture_to_delete = (ResourceTexture*)App->resources->GetResourceWithID(ID);
			if (texture_to_delete != nullptr) {
				App->objects->GetRoot(true)->SearchResourceToDelete(ResourceType::RESOURCE_TEXTURE, (Resource*)texture_to_delete);
			}
			break; }
		case FileDropType::MODEL3D: {
			std::string path_ = App->file_system->GetPathWithoutExtension(path + name);
			path_ += "_meta.alien";
			u64 ID = App->resources->GetIDFromAlienPath(path_.data());
			ResourceModel* model_to_delete = (ResourceModel*)App->resources->GetResourceWithID(ID);
			if (model_to_delete != nullptr) {
				for (uint i = 0; i < model_to_delete->meshes_attached.size(); ++i) {
					if (model_to_delete->meshes_attached[i] != nullptr) {
						App->objects->GetRoot(true)->SearchResourceToDelete(ResourceType::RESOURCE_MESH, (Resource*)model_to_delete->meshes_attached[i]);
					}
				}
			}
			break; }
		case FileDropType::PREFAB: {
			std::string path_ = App->file_system->GetPathWithoutExtension(path + name);
			path_ += ".alienPrefab";
			u64 ID = App->resources->GetIDFromAlienPath(path_.data());
			ResourcePrefab* prefab = (ResourcePrefab*)App->resources->GetResourceWithID(ID);
			if (prefab != nullptr) {
				auto item = prefab->prefab_references.begin();
				for (; item != prefab->prefab_references.end(); ++item) {
					if (*item != nullptr) {
						(*item)->prefabID = 0;
						(*item)->prefab_name.clear();
					}
				}
			}
			break; }
		}
	}
	else {
		for (uint i = 0; i < children.size(); ++i) {
			children[i]->RemoveResourceOfGameObjects();
		}
	}
}

void FileNode::SetIcon()
{
	if (is_file) {
		std::string extension;
		App->file_system->SplitFilePath(std::string(path + name).data(), nullptr, nullptr, &extension);

		if (App->StringCmp(extension.data(), "jpg")) {
			icon = App->resources->icons.jpg_file;
			type = FileDropType::TEXTURE;
		}
		else if (App->StringCmp(extension.data(), "dds")) {
			icon = App->resources->icons.dds_file;
			type = FileDropType::TEXTURE;
		}
		else if (App->StringCmp(extension.data(), "png")) {
			icon = App->resources->icons.png_file;
			type = FileDropType::TEXTURE;
		}
		else if (App->StringCmp(extension.data(), "tga")) {
			icon = App->resources->icons.tga_file;
			type = FileDropType::TEXTURE;
		}
		else if (App->StringCmp(extension.data(), "fbx")) {
			icon = App->resources->icons.model;
			type = FileDropType::MODEL3D;
		}
		else if (App->StringCmp(extension.data(), "alienScene")) {
			icon = App->resources->icons.scene_file;
			type = FileDropType::SCENE;
		}
		else if (App->StringCmp(extension.data(), "alienPrefab")) {
			icon = App->resources->icons.prefab_icon;
			type = FileDropType::PREFAB;
		}
		else if (App->StringCmp(extension.data(), "alienScript")) {
			icon = App->resources->icons.script_file;
			type = FileDropType::SCRIPT;
		}
		else {
			// TODO: fer un icon que sigui unknown
			icon = App->resources->icons.model;
		}
	}
	else {
		icon = App->resources->icons.folder;
		type = FileDropType::FOLDER;
	}
}
