#include "Color.h"
#include "GameObject.h"
#include "glew/include/glew.h"
#include "Application.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentLight.h"
#include "RandomHelper.h"

GameObject::GameObject(GameObject* parent)
{
	id = GetRandomIntBetweenTwo(INT_MIN, INT_MAX);

	if (parent != nullptr) {
		this->parent = parent;
		parent->AddChild(this);
	}
}

GameObject::~GameObject()
{
	if (App->objects->GetSelectedObject() == this)
		App->objects->DeselectObject();

	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item) {
		if (*item != nullptr) {
			delete* item;
			*item = nullptr;
		}
	}

	std::vector<GameObject*>::iterator child = children.begin();
	for (; child != children.end(); ++child) {
		if (*child != nullptr) {
			delete* child;
			*child = nullptr;
		}
	}
}

bool GameObject::IsEnabled()
{
	return enabled;
}

void GameObject::Draw()
{
	ComponentMaterial* material = (ComponentMaterial*)GetComponent(ComponentType::MATERIAL);
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);
	ComponentLight* light = (ComponentLight*)GetComponent(ComponentType::LIGHT);

	if (material != nullptr && material->IsEnabled() && mesh != nullptr && mesh->IsEnabled()) 
	{
		material->BindTexture();
	}

	if (mesh != nullptr && mesh->IsEnabled()) 
	{
		if (material == nullptr || (material != nullptr && !material->IsEnabled())) // set the basic color if the GameObject hasn't a material
			glColor3f(1, 1, 1);
		if (!mesh->wireframe)
			mesh->DrawPolygon();
		if ((selected || parent_selected) && App->objects->outline)
			mesh->DrawOutLine();
		if (mesh->view_mesh || mesh->wireframe)
			mesh->DrawMesh();
		if (mesh->view_vertex_normals)
			mesh->DrawVertexNormals();
		if (mesh->view_face_normals)
			mesh->DrawFaceNormals();
	}


	//if (mesh != nullptr) {

	//	float height = abs(mesh->aabb_max.y - mesh->aabb_min.y);
	//	float width = abs(mesh->aabb_max.x - mesh->aabb_min.x);
	//	float prof = abs(mesh->aabb_max.z - mesh->aabb_min.z);

	//	glColor3f(1,0,0);
	//	glBegin(GL_LINES);

	//
	//	glVertex3f(mesh->aabb_min.x, mesh->aabb_min.y, mesh->aabb_min.z);
	//	glVertex3f(mesh->aabb_max.x, mesh->aabb_min.y, mesh->aabb_min.z);

	//	glVertex3f(mesh->aabb_min.x, mesh->aabb_min.y, mesh->aabb_min.z);
	//	glVertex3f(mesh->aabb_min.x, mesh->aabb_min.y, mesh->aabb_max.z);

	//	glVertex3f(mesh->aabb_min.x, mesh->aabb_min.y, mesh->aabb_min.z);
	//	glVertex3f(mesh->aabb_min.x, mesh->aabb_max.y, mesh->aabb_min.z);

	//	//

	//	glVertex3f(mesh->aabb_max.x, mesh->aabb_min.y, mesh->aabb_min.z);
	//	glVertex3f(mesh->aabb_max.x, mesh->aabb_max.y, mesh->aabb_min.z);

	//	glVertex3f(mesh->aabb_max.x, mesh->aabb_min.y, mesh->aabb_min.z);
	//	glVertex3f(mesh->aabb_max.x, mesh->aabb_min.y, mesh->aabb_max.z);

	//	glVertex3f(mesh->aabb_min.x, mesh->aabb_max.y, mesh->aabb_min.z);
	//	glVertex3f(mesh->aabb_min.x, mesh->aabb_max.y, mesh->aabb_max.z);
	//	
	//	glVertex3f(mesh->aabb_min.x, mesh->aabb_max.y, mesh->aabb_min.z);
	//	glVertex3f(mesh->aabb_max.x, mesh->aabb_max.y, mesh->aabb_min.z);

	//	glVertex3f(mesh->aabb_max.x, mesh->aabb_max.y, mesh->aabb_min.z);
	//	glVertex3f(mesh->aabb_max.x, mesh->aabb_max.y, mesh->aabb_max.z);

	//	glVertex3f(mesh->aabb_max.x, mesh->aabb_min.y, mesh->aabb_max.z);
	//	glVertex3f(mesh->aabb_max.x, mesh->aabb_max.y, mesh->aabb_max.z);

	//	glVertex3f(mesh->aabb_min.x, mesh->aabb_min.y, mesh->aabb_max.z);
	//	glVertex3f(mesh->aabb_max.x, mesh->aabb_min.y, mesh->aabb_max.z);

	//	glVertex3f(mesh->aabb_min.x, mesh->aabb_min.y, mesh->aabb_max.z);
	//	glVertex3f(mesh->aabb_min.x, mesh->aabb_max.y, mesh->aabb_max.z);

	//	glVertex3f(mesh->aabb_min.x, mesh->aabb_max.y, mesh->aabb_max.z);
	//	glVertex3f(mesh->aabb_max.x, mesh->aabb_max.y, mesh->aabb_max.z);

	//	glEnd();
	//}


	if (light != nullptr && light->IsEnabled()) 
	{
		light->LightLogic();
	}

	std::vector<GameObject*>::iterator child = children.begin();
	for (; child != children.end(); ++child) {
		if (*child != nullptr && (*child)->IsEnabled()) {
			(*child)->Draw();
		}
	}
}

void GameObject::AddComponent(Component* component)
{
	bool exists = false;
	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item) {
		if (*item != nullptr && (*item)->GetType() == component->GetType())
		{
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		components.push_back(component);
	}
}

bool GameObject::HasComponent(ComponentType component)
{
	bool exists = false;

	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item) {
		if (*item != nullptr && (*item)->GetType() == component)
		{
			exists = true;
			break;
		}
	}

	return exists;
}

void GameObject::AddChild(GameObject* child)
{
	children.push_back(child);
}

void GameObject::SetName(const char* name)
{
	this->name = std::string(name);
}

const char* GameObject::GetName()
{
	return name.c_str();
}

Component* GameObject::GetComponent(const ComponentType& type)
{
	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item) {
		if (*item != nullptr && (*item)->GetType() == type) {
			return *item;
		}
	}
	return nullptr;
}

bool GameObject::IsSelected()
{
	return selected;
}

bool GameObject::IsParentSelected()
{
	return parent_selected;
}

void GameObject::ChangeSelected(const bool& select)
{
	selected = select;

	SayChildrenParentIsSelected(selected);
}

void GameObject::ChangeWireframe(const bool& wireframe)
{
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);

	if (mesh != nullptr)
		mesh->wireframe = wireframe;

	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->ChangeWireframe(wireframe);
		}
	}
}

void GameObject::ChangeMeshView(const bool& mesh_view)
{
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);

	if (mesh != nullptr)
		mesh->view_mesh = mesh_view;

	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->ChangeMeshView(mesh_view);
		}
	}
}

void GameObject::ChangeVertexNormalsView(const bool& normals)
{
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);

	if (mesh != nullptr)
		mesh->view_vertex_normals = normals;

	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->ChangeVertexNormalsView(normals);
		}
	}
}

void GameObject::ChangeFaceNormalsView(const bool& normals)
{
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);

	if (mesh != nullptr)
		mesh->view_face_normals = normals;

	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->ChangeFaceNormalsView(normals);
		}
	}
}

bool GameObject::HasChildren()
{
	return !children.empty();
}

void GameObject::SetNewParent(GameObject* new_parent)
{
	if (new_parent != nullptr && !Exists(new_parent)) {
		GameObject* last_parent = parent;
		parent->children.erase(std::find(parent->children.begin(), parent->children.end(), this));
		parent = new_parent;
		parent->AddChild(this);

		// TODO: recalculate transformations

		ComponentTransform* parent_transform = (ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM);
		ComponentTransform* transform = (ComponentTransform*)GetComponent(ComponentType::TRANSFORM);

		if (parent_transform != nullptr) {
			// transform global del pare - transform global del obj daqui i el que doni es suma a la local i gg


			//float3 pos = { parent_transform->GetGlobalPosition().x + transform->GetLocalPosition().x - transform->GetGlobalPosition().x,
			//parent_transform->GetGlobalPosition().y + transform->GetLocalPosition().y - transform->GetGlobalPosition().y,
			//parent_transform->GetGlobalPosition().z + transform->GetLocalPosition().z - transform->GetGlobalPosition().z };

			//transform->SetLocalPosition(transform->GetLocalPosition().x + pos.x,
			//transform->GetLocalPosition().y + pos.y,
			//transform->GetLocalPosition().z + pos.z);

			//transform->global_transformation = parent_transform->global_transformation * transform->local_transformation;
		}
		else {
			ComponentTransform* last_parent_transform = (ComponentTransform*)last_parent->GetComponent(ComponentType::TRANSFORM);
			if (last_parent_transform != nullptr) {
				transform->global_transformation = last_parent_transform->global_transformation + transform->local_transformation;
				transform->local_transformation = transform->global_transformation;
			}
		}

	}
}

void GameObject::ToDelete()
{
	to_delete = true;
	App->objects->need_to_delete_objects = true;
}

void GameObject::SayChildrenParentIsEnabled(const bool& enabled)
{
	if (enabled)
		OnEnable();
	else
		OnDisable();

	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->parent_enabled = enabled;
			(*item)->SayChildrenParentIsEnabled(enabled);
		}
	}
}

bool GameObject::IsParentEnabled()
{
	return parent_enabled;
}

void GameObject::OnEnable()
{
	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item) {
		if (*item != nullptr) {
			(*item)->OnEnable();
		}
	}

	std::vector<GameObject*>::iterator it = children.begin();
	for (; it != children.end(); ++it) {
		if (*it != nullptr) {
			(*it)->OnEnable();
		}
	}
}

void GameObject::OnDisable()
{
	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item) {
		if (*item != nullptr) {
			(*item)->OnDisable();
		}
	}

	std::vector<GameObject*>::iterator it = children.begin();
	for (; it != children.end(); ++it) {
		if (*it != nullptr) {
			(*it)->OnDisable();
		}
	}
}

void GameObject::ScaleNegative(const bool& is_negative)
{
	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			ComponentTransform* tr = (ComponentTransform*)(*item)->GetComponent(ComponentType::TRANSFORM);
			if (tr != nullptr)
				tr->SetScaleNegative(is_negative);
			(*item)->ScaleNegative(is_negative);
		}
	}
}

void GameObject::SayChildrenParentIsSelected(const bool& selected)
{
	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->parent_selected = selected;
			(*item)->SayChildrenParentIsSelected(selected);
		}
	}
}

GameObject* GameObject::GetGameObjectByID(const int& id)
{
	GameObject* ret = nullptr;
	if (id == this->id) {
		return this;
	}
	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			ret = (*item)->GetGameObjectByID(id);
			if (ret != nullptr)
				break;
		}
	}
	return ret;
}

bool GameObject::Exists(GameObject* object)
{
	bool ret = false;

	if (this == object)
		return true;

	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			ret = (*item)->Exists(object);
			if (ret)
				break;
		}
	}

	return ret;
}

void GameObject::SearchToDelete()
{
	std::vector<GameObject*>::iterator item = children.begin();
	while (item != children.end()) {

		if ((*item)->to_delete) {
			delete* item;
			*item = nullptr;
			item = children.erase(item);
		}
		else {

			std::vector<Component*>::iterator item_com = (*item)->components.begin();
			while (item_com != (*item)->components.end())
			{
				if ((*item_com) != nullptr && (*item_com)->not_destroy != true)
				{
					delete* item_com;
					*item_com = nullptr;
					item_com = (*item)->components.erase(item_com);
					
				}
				else
				{
					++item_com;
				}
			}
			(*item)->SearchToDelete();
			++item;
		}
	}
}





