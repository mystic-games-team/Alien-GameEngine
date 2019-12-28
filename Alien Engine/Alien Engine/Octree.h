#pragma once

#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>
#include <list>
#include <map>
#include "GameObject.h"

class ComponentCamera;

class OctreeNode {

	friend class Octree;

public:

	OctreeNode(const float3& min, const float3& max);
	~OctreeNode();

	// insert a gameobject
	void Insert(GameObject* object, const AABB& sect);
	
	// remove a gameobject
	bool Remove(GameObject* object);
	// draw AABB
	void DrawNode();

private:

	void AddGameObject(GameObject* obj);
	void AddNode(const float3& min, const float3& max);
	void Subdivide();
	void Regrup();
	void SaveGameObjects(std::vector<GameObject*>* to_save, AABB* new_section);
	bool AddToChildren(GameObject* obj, const AABB& sect);
	void SetStaticDrawList(std::vector<std::pair<float, GameObject*>>* to_draw, const ComponentCamera* camera);

public:

	AABB section;
	std::vector<GameObject*> game_objects;
	OctreeNode* parent = nullptr;
	std::vector<OctreeNode*> children;

};

class Octree {

public:

	Octree();
	~Octree();
	
	// insert a gameobject
	void Insert(GameObject* object, bool add_children);
	// remove a gameobject
	void Remove(GameObject* object);
	// remove the hole octree
	void Clear();

	// draw
	void Draw();

	const uint& GetBucket() const;
	void SetBucket(const uint& bucket);

	// init the octree with min and max
	void Init(const float3& min, const float3& max);
	bool Exists(GameObject* object);
	// create again the octree
	void Recalculate(GameObject* new_object);

	void SetStaticDrawList(std::vector<std::pair<float, GameObject*>>* to_draw, const ComponentCamera* camera);

	uint bucket = 2;
	OctreeNode* root = nullptr;
private:


	void RemoveRecursively(GameObject* obj);

private:

	std::list<GameObject*> all_objects;

};