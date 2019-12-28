#pragma once

#include "Resource_.h"
#include <vector>
#include "MathGeoLib/include/Math/float3.h"
#include <list>

class Prefab;
class ResourceMesh;
class GameObject;


class ResourceScene : public Resource {

public:

	ResourceScene();
	virtual ~ResourceScene();

	// meta data
	bool CreateMetaData(const u64& force_id = 0);
	bool ReadBaseInfo(const char* assets_file_path);
	void ReadLibrary(const char* meta_data);
	bool DeleteMetaData();

private:

};