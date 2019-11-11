#include "Octree.h"
#include "ComponentMesh.h"
#include "ModuleObjects.h"
#include "Application.h"

OctreeNode::OctreeNode()
{
}

OctreeNode::OctreeNode(const float3& min, const float3& max)
{
	section.minPoint = min;
	section.maxPoint = max;
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::Insert(GameObject* object, const AABB& sect)
{
	bool intersects = section.Intersects(sect);
	bool contains = section.Contains(sect);

	if (contains) {  
		if (children.empty()) {
			if (game_objects.size() < App->objects->octree.GetBucket()) {
				AddGameObject(object);
			}
			else {
				Subdivide();
			}
		}
		if (!children.empty()) {
			std::vector<OctreeNode*>::iterator item = children.begin();
			uint intersections = 0;
			for (; item != children.end(); ++item) {
				if (*item != nullptr && (*item)->section.Contains(sect)) {
					(*item)->Insert(object, sect);
					break;
				}
				if (*item != nullptr && (*item)->section.Intersects(sect)) {
					++intersections;
				}
			}
			if (intersections > 1) { // if objects intersects with more than one child, obj belongs to the parent now
				AddGameObject(object);
			}
		}
	}
	else if (intersects) { //
		
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

	if (!children.empty()) {
		std::vector<OctreeNode*>::iterator item = children.begin();
		for (; item != children.end(); ++item) {
			if (*item != nullptr) {
				(*item)->DrawNode();
			}
		}
	}
}

void OctreeNode::AddGameObject(GameObject* obj)
{
	game_objects.push_back(obj);
}

void OctreeNode::AddNode(const float3& min, const float3& max)
{
	OctreeNode* node = new OctreeNode(min, max);
	node->parent = this;
	children.push_back(node);
}

void OctreeNode::Subdivide()
{
	float3 mid_point = section.minPoint + (section.maxPoint - section.minPoint) * 0.5F;

	AddNode(section.minPoint, mid_point);
	AddNode(float3{ section.minPoint.x,section.maxPoint.y,section.minPoint.z }, mid_point);

	AddNode(mid_point, section.maxPoint);
	AddNode(mid_point, float3{ section.maxPoint.x,section.minPoint.y,section.maxPoint.z });

	AddNode(float3{ section.maxPoint.x,section.maxPoint.y,section.minPoint.z }, mid_point);
	AddNode(float3{ section.maxPoint.x,section.minPoint.y,section.minPoint.z }, mid_point);

	AddNode(mid_point, float3{ section.minPoint.x,section.maxPoint.y,section.maxPoint.z });
	AddNode(mid_point, float3{ section.minPoint.x,section.minPoint.y,section.maxPoint.z });
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
	root = new OctreeNode(min, max);
}

bool Octree::IsRoot(const OctreeNode* node)
{
	return node == root;
}
