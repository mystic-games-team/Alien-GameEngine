#pragma once

#include <string>
#include <vector>
#include "ModuleFileSystem.h"

class ResourceTexture;
enum class FileDropType;

class FileNode {

public:

	FileNode();
	FileNode(std::string name, bool is_file, FileNode* parent);
	FileNode(const std::string& path, const std::string& name, bool is_file, FileNode* parent);

	~FileNode();

	std::string name;
	std::string path;

	bool is_file = true;
	bool is_base_file = false;

	bool changing_name = false;

	FileNode* parent = nullptr;
	std::vector<FileNode*> children;

	ResourceTexture* icon = nullptr;

	FileDropType type = FileDropType::UNKNOWN;

	void DeleteChildren();
	void DeleteNodeData();

	FileNode* FindChildrenByPath(const std::string& path); // just children, NO children of children!!!

private:

	void SetIcon();

};
