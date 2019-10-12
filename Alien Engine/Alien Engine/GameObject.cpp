#include "Color.h"
#include "GameObject.h"
#include "glew/include/glew.h"
#include "Application.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"


GameObject::GameObject()
{

}

GameObject::~GameObject()
{
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

	if (material != nullptr) {
		material->BindTexture();
	}

	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);

	if (mesh != nullptr) {
		if (material == nullptr) // set the basic color if the GameObject hasn't a material
			glColor3f(1, 1, 1);
		if (!App->objects->wireframe_mode)
			mesh->DrawPolygon();
		if (App->objects->wireframe_mode || App->objects->view_mesh_mode)
			mesh->DrawMesh();
		if (App->objects->draw_vertex_normals)
			mesh->DrawVertexNormals();
		if (App->objects->draw_face_normals)
			mesh->DrawFaceNormals();
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
		if (*item != nullptr && (*item)->GetType() == component->GetType()) {
			exists = true;
			break;
		}
	}
	if (!exists) {
		component->game_object_attached = this;
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
	LOG("No component found");
	return nullptr;
}




