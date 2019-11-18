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
	bool CreateMetaData(GameObject* object);
	bool ReadBaseInfo(const char* assets_file_path);
	bool DeleteMetaData();

	// create GameObjects
	void ConvertToGameObjects();
};