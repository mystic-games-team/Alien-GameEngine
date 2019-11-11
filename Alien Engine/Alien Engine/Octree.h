#pragma once

#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>
#include "GameObject.h"

class OctreeNode {

public:

	OctreeNode();
	~OctreeNode();

	// insert a gameobject
	void Insert(GameObject* object, const AABB& sect);
	// remove a gameobject
	void Remove(GameObject* object);
	// draw AABB
	void DrawNode();

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
	void Insert(GameObject* object);
	// remove a gameobject
	void Remove(GameObject* object);
	// remove the hole octree
	void Clear();

	// draw
	void Draw();

	const uint& GetBucket() const;

	void Init(const float3& min, const float3& max);

private:

	OctreeNode* root = nullptr;
	uint bucket = 1;
};