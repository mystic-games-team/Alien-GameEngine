#include "Octree.h"
#include "ComponentMesh.h"
#include "ModuleObjects.h"
#include "Application.h"

OctreeNode::OctreeNode()
{
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::Insert(GameObject* object, const AABB& sect)
{
	bool intersects = section.Intersects(sect); 
	bool contains = section.Contains(sect);

	if (intersects && contains) { // sect == section

	}
	else if (intersects) { // 

	}
	else if (contains) { //

	}
	else { // need to make the octree again 

	}

	if (children.size() < App->objects->octree.GetBucket()) { // we can insert it here
		
	}
	else { // need to subdivide

	}

}

void OctreeNode::Remove(GameObject* object)
{
}

void OctreeNode::DrawNode()
{
	glColor3f(1, 0, 0);
	glLineWidth(1);
	glBegin(GL_LINES);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.minPoint.y, section.minPoint.z);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.minPoint.x, section.minPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.minPoint.x, section.maxPoint.y, section.minPoint.z);

	glVertex3f(section.maxPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.minPoint.z);

	glVertex3f(section.maxPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.minPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.maxPoint.y, section.minPoint.z);
	glVertex3f(section.minPoint.x, section.maxPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.maxPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.minPoint.z);

	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.maxPoint.z);

	glVertex3f(section.maxPoint.x, section.minPoint.y, section.maxPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.maxPoint.z);
	glVertex3f(section.maxPoint.x, section.minPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.maxPoint.z);
	glVertex3f(section.minPoint.x, section.maxPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.maxPoint.y, section.maxPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.maxPoint.z);

	glEnd();

	glLineWidth(1);
}

Octree::Octree()
{
}

Octree::~Octree()
{
}

void Octree::Insert(GameObject* object)
{
	ComponentMesh* mesh_parent = (ComponentMesh*)object->GetComponent(ComponentType::MESH);
	if (mesh_parent != nullptr && mesh_parent->mesh != nullptr) {
		root->Insert(object, mesh_parent->GetGlobalAABB());
	}

	if (!object->children.empty()) {
		std::vector<GameObject*>::iterator item = object->children.begin();
		for (; item != object->children.end(); ++item) {
			if (*item != nullptr) {
				Insert((*item));
			}
		}
	}
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

void Octree::Draw()
{
	if (root != nullptr)
		root->DrawNode();
}

const uint& Octree::GetBucket() const
{
	return bucket;
}

void Octree::Init(const float3& min, const float3& max)
{
	if (root != nullptr) {
		delete root;
	}
	root = new OctreeNode();
	root->section.minPoint = min;
	root->section.maxPoint = max;
}
