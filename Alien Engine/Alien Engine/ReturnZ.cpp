#include "ReturnZ.h"
#include "Application.h"
#include "ModuleObjects.h"
#include "ComponentTransform.h"
#include "ResourceMesh.h"

void ReturnZ::SetAction(const ReturnActions& type, void* data)
{
	this->type = type;

	switch (type)
	{
	case ReturnActions::DELETE_OBJECT: {
		ReturnZ::SetDeleteObject((GameObject*)data, &object);
		break; }
	default:
		break;
	}
}

void ReturnZ::AddNewAction(const ReturnActions& type, void* data)
{
	ReturnZ* ret = new ReturnZ();
	ret->SetAction(type, data);
	App->objects->return_actions.push(ret);
}

void ReturnZ::GoBackOneAction()
{
	ReturnZ* to_return = App->objects->return_actions.top();

	switch (to_return->type) {
	case ReturnActions::DELETE_OBJECT: {
		ReturnZ::CreateObject(&to_return->object);
		break; }
	}

	App->objects->return_actions.pop();
}

void ReturnZ::SetDeleteObject(GameObject* obj, Obj* to_fill)
{
	to_fill->enabled = obj->enabled;
	to_fill->is_static = obj->is_static;
	to_fill->ID = obj->ID;
	to_fill->parentID = obj->parent->ID;
	to_fill->parent_selected = obj->IsParentSelected();
	to_fill->selected = obj->IsSelected();
	to_fill->parent_enabled = obj->IsParentEnabled();
	to_fill->name = std::string(obj->GetName()).data();
	
	if (!obj->components.empty()) {
		std::vector<Component*>::iterator item = obj->components.begin();
		for (; item != obj->components.end(); ++item) {
			if (*item != nullptr) {
				Obj::Comp* comp = new Obj::Comp();
				switch ((*item)->GetType()) {
				case ComponentType::TRANSFORM: {
					ComponentTransform* transform = (ComponentTransform*)obj->GetComponent(ComponentType::TRANSFORM);
					comp->transform.pos = transform->GetLocalPosition();
					comp->transform.scale = transform->GetLocalScale();
					comp->transform.rot = transform->GetLocalRotation();
					comp->transform.is_scale_negative = transform->IsScaleNegative();
					comp->type = ComponentType::TRANSFORM;
					to_fill->comps.push_back(comp);
					break; }
				case ComponentType::MESH: {
					ComponentMesh* mesh = (ComponentMesh*)obj->GetComponent(ComponentType::MESH);
					if (mesh->mesh != nullptr)
						comp->mesh.ID = mesh->mesh->GetID();
					comp->mesh.draw_AABB = mesh->draw_AABB;
					comp->mesh.draw_OBB = mesh->draw_OBB;
					comp->mesh.view_face_normals = mesh->view_face_normals;
					comp->mesh.view_vertex_normals = mesh->view_vertex_normals;
					comp->mesh.wireframe = mesh->wireframe;
					comp->mesh.view_mesh = mesh->view_mesh;
					comp->type = ComponentType::MESH;
					to_fill->comps.push_back(comp);
					break; }
				}
			}
		}
	}

	if (!obj->children.empty()) {
		std::vector<GameObject*>::iterator item = obj->children.begin();
		for (; item != obj->children.end(); ++item) {
			if (*item != nullptr) {
				Obj* obj = new Obj();
				ReturnZ::SetDeleteObject((*item), obj);
				to_fill->children.push_back(obj);
			}
		}
	}
}

void ReturnZ::CreateObject(Obj* obj)
{
	GameObject* new_obj = new GameObject();
	new_obj->parent = App->objects->GetGameObjectByID(obj->parentID);
	if (new_obj->parent != nullptr) {
		new_obj->parent->AddChild(new_obj);
	}
	new_obj->enabled = obj->enabled;
	new_obj->is_static = obj->is_static;
	if (new_obj->is_static) {
		App->objects->octree.Insert(new_obj, false);
	}
	new_obj->ID = obj->ID;
	new_obj->SetName(obj->name.data());
	if (obj->selected) {
		App->objects->SetNewSelectedObject(new_obj);
	}
	new_obj->parent_enabled = obj->parent_enabled;
	new_obj->parent_selected = obj->parent_selected;

	if (!obj->comps.empty()) {
		std::vector<Obj::Comp*>::iterator item = obj->comps.begin();
		for (; item != obj->comps.end(); ++item) {
			if (*item != nullptr) {
				SDL_assert((uint)ComponentType::UNKNOWN == 4); // add new type to switch
				switch ((*item)->type)
				{
				case ComponentType::TRANSFORM: {
					new_obj->AddComponent(new ComponentTransform(new_obj, (*item)->transform.pos, (*item)->transform.rot, (*item)->transform.scale));
					break; }
				case ComponentType::MESH: {
					ComponentMesh* mesh = new ComponentMesh(new_obj);
					if ((*item)->mesh.ID != 0)
						mesh->mesh = (ResourceMesh*)App->resources->GetResourceWithID((*item)->mesh.ID);
					mesh->draw_AABB = (*item)->mesh.draw_AABB;
					mesh->draw_OBB = (*item)->mesh.draw_OBB;
					mesh->wireframe = (*item)->mesh.wireframe;
					mesh->view_mesh = (*item)->mesh.view_mesh;
					mesh->view_face_normals = (*item)->mesh.view_face_normals;
					mesh->view_vertex_normals = (*item)->mesh.view_vertex_normals;
					new_obj->AddComponent(mesh);
					break; }
				default:
					break;

				}
			}
		}
	}

	if (!obj->children.empty()) {
		std::vector<Obj*>::iterator item = obj->children.begin();
		for (; item != obj->children.end(); ++item) {
			if (*item != nullptr) {
				ReturnZ::CreateObject((*item));
			}
		}
	}


}
