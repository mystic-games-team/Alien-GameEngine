#include "Color.h"
#include "GameObject.h"
#include "glew/include/glew.h"
#include "Application.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentLight.h"

GameObject::GameObject()
{

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

void GameObject::Enable()
{
	enabled = true;

	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->Enable();
		}
	}
}

void GameObject::Disable()
{
	enabled = false;

	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->Disable();
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

void GameObject::ToDelete()
{
	to_delete = true;
	App->objects->need_to_delete_objects = true;
}

void GameObject::SayChildrenParentIsEnabled(const bool& enabled)
{
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
			(*item)->SearchToDelete();
			++item;
		}
	}
}




