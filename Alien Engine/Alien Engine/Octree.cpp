#include "Octree.h"

OctreeNode::OctreeNode()
{
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::Insert(GameObject* object)
{
}

void OctreeNode::Remove(GameObject* object)
{
}

void OctreeNode::DrawNode()
{
}

void Octree::Insert(GameObject* object)
{
}

void Octree::Remove(GameObject* object)
{
}

void Octree::Clear()
{
	if (root != nullptr) {
		delete root;
		root = nullptr;
	}
}
