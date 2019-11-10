#include "FileNode.h"
#include "Application.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

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

		remove(hole_path.data());

		std::string meta_path = App->file_system->GetPathWithoutExtension(hole_path.data()) + "_meta.alien";

		u64 ID = App->resources->GetIDFromAlienPath(meta_path.data());

		remove(meta_path.data());

		Resource* resource_to_delete = App->resources->GetResourceWithID(ID);

		resource_to_delete->DeleteMetaData();
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
			type = FileDropType::TEXTURE;;
		}
		else if (App->StringCmp(extension.data(), "fbx")) {
			icon = App->resources->icons.model;
			type = FileDropType::MODEL3D;
		}
		else if (App->StringCmp(extension.data(), "alienScene")) {
			icon = App->resources->icons.scene_file;
			type = FileDropType::SCENE;
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
