#pragma once

#include "MathGeoLib/include/Math/float3.h"

typedef unsigned long long u64;

class GameObject;

class __declspec(dllexport) Prefab {
	friend class ComponentScript;
	friend class FileNode;
	friend class ModuleObjects;
	friend class GameObject;
	friend class PanelScene;
public:

	Prefab();
	~Prefab();

	// parent = nullptr set the root
	GameObject* ConvertToGameObject(float3 local_position, GameObject * parent = nullptr);

private:

	static void InitScripts(GameObject* obj);

private:
	u64 prefabID = 0;
	std::string prefab_name;
};