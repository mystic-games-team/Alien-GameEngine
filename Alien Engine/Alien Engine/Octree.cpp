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

	AddNode(section.minPoint, mid_point); // 0
	AddNode(mid_point, section.maxPoint); // 2

	float3 point = { section.minPoint.x,section.maxPoint.y,section.minPoint.z };

	AddNode(float3{min(point.x,mid_point.x), min(point.y,mid_point.y), min(point.z,mid_point.z) },
		float3{ max(point.x,mid_point.x), max(point.y,mid_point.y), max(point.z,mid_point.z) });

	point = { section.maxPoint.x,section.minPoint.y,section.maxPoint.z };

	AddNode(float3{ min(point.x,mid_point.x), min(point.y,mid_point.y), min(point.z,mid_point.z) },
		float3{ max(point.x,mid_point.x), max(point.y,mid_point.y), max(point.z,mid_point.z) });

	point = { section.maxPoint.x,section.maxPoint.y,section.minPoint.z };

	AddNode(float3{ min(point.x,mid_point.x), min(point.y,mid_point.y), min(point.z,mid_point.z) },
		float3{ max(point.x,mid_point.x), max(point.y,mid_point.y), max(point.z,mid_point.z) });

	point = { section.maxPoint.x,section.minPoint.y,section.minPoint.z };

	AddNode(float3{ min(point.x,mid_point.x), min(point.y,mid_point.y), min(point.z,mid_point.z) },
		float3{ max(point.x,mid_point.x), max(point.y,mid_point.y), max(point.z,mid_point.z) });

	point = { section.minPoint.x,section.maxPoint.y,section.maxPoint.z };

	AddNode(float3{ min(point.x,mid_point.x), min(point.y,mid_point.y), min(point.z,mid_point.z) },
		float3{ max(point.x,mid_point.x), max(point.y,mid_point.y), max(point.z,mid_point.z) });

	point = { section.minPoint.x,section.minPoint.y,section.maxPoint.z };
	
	AddNode(float3{ min(point.x,mid_point.x), min(point.y,mid_point.y), min(point.z,mid_point.z) },
		float3{ max(point.x,mid_point.x), max(point.y,mid_point.y), max(point.z,mid_point.z) });

	// reorder the gameobjects to the new nodes if possible, if intersects parent keep them
	std::vector<GameObject*>::iterator objs = game_objects.begin();
	while (objs != game_objects.end()) {
		bool to_delete = false;
		if (*objs != nullptr) {
			std::vector<OctreeNode*>::iterator item = children.begin();
			uint intersections = 0;
			for (; item != children.end(); ++item) {
				ComponentMesh* mesh = (ComponentMesh*)(*objs)->GetComponent(ComponentType::MESH);
				if (*item != nullptr && (*item)->section.Contains(mesh->GetGlobalAABB().minPoint) && (*item)->section.Contains(mesh->GetGlobalAABB().maxPoint)) {
					(*item)->Insert(*objs, mesh->GetGlobalAABB());
					to_delete = true;
					break;
				}
			}
		}
		if (to_delete) {
			objs = game_objects.erase(objs);
		}
		else {
			++objs;
		}
	}

	
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
