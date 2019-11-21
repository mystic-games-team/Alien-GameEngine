#pragma once

#include "Resource_.h"
#include <vector>

class ResourceMesh;
class GameObject;

class ResourcePrefab : public Resource {

public:

	ResourcePrefab();
	virtual ~ResourcePrefab();

	// meta data
	bool CreateMetaData(GameObject* object, const char* folder = nullptr);
	bool ReadBaseInfo(const char* assets_file_path);
	bool DeleteMetaData();

	void OpenPrefabScene();

	// create GameObjects
	void ConvertToGameObjects(int list_num = -1);
};