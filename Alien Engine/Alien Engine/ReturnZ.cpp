#include "ReturnZ.h"
#include "Application.h"
#include "ModuleObjects.h"
#include "ComponentTransform.h"
#include "ResourceMesh.h"
#include "ComponentMaterial.h"
#include "ResourceTexture.h"

void ReturnZ::SetAction(const ReturnActions& type, void* data)
{
	switch (type)
	{
	case ReturnActions::DELETE_OBJECT: {
		ActionDeleteObject* object = new ActionDeleteObject();
		ReturnZ::SetDeleteObject((GameObject*)data, object);
		action = object;
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

	switch (to_return->action->type) {
	case ReturnActions::DELETE_OBJECT: {
		ActionDeleteObject* object = (ActionDeleteObject*)to_return->action;
		ReturnZ::CreateObject(object);
		break; }
	}

	App->objects->return_actions.pop();
}

void ReturnZ::SetDeleteObject(GameObject* obj, ActionDeleteObject* to_fill)
{
	to_fill->type = ReturnZ::ReturnActions::DELETE_OBJECT;
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
				ActionDeleteObject::Comp* comp = new ActionDeleteObject::Comp();
				comp->type = (*item)->GetType();
				switch ((*item)->GetType()) {
				case ComponentType::TRANSFORM: {
					ComponentTransform* transform = (ComponentTransform*)obj->GetComponent(ComponentType::TRANSFORM);
					comp->transform.pos = transform->GetLocalPosition();
					comp->transform.scale = transform->GetLocalScale();
					comp->transform.rot = transform->GetLocalRotation();
					comp->transform.is_scale_negative = transform->IsScaleNegative();
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
					break; }
				case ComponentType::MATERIAL: {
					ComponentMaterial* material = (ComponentMaterial*)obj->GetComponent(ComponentType::MATERIAL);
					if (material->texture != nullptr)
						comp->material.ID = material->texture->GetID();
					comp->material.color = material->color;
					comp->material.texture_activated = material->texture_activated;
					break; }
				}
				if (comp != nullptr) {
					to_fill->comps.push_back(comp);
				}
			}
		}
	}

	if (!obj->children.empty()) {
		std::vector<GameObject*>::iterator item = obj->children.begin();
		for (; item != obj->children.end(); ++item) {
			if (*item != nullptr) {
				ActionDeleteObject* obj = new ActionDeleteObject();
				ReturnZ::SetDeleteObject((*item), obj);
				to_fill->children.push_back(obj);
			}
		}
	}
}

void ReturnZ::CreateObject(ActionDeleteObject* obj)
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
		std::vector<ActionDeleteObject::Comp*>::iterator item = obj->comps.begin();
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
				case ComponentType::MATERIAL: {
					ComponentMaterial* material = new ComponentMaterial(new_obj);
					if ((*item)->material.ID != 0)
						material->texture = (ResourceTexture*)App->resources->GetResourceWithID((*item)->material.ID);
					material->texture_activated = (*item)->material.texture_activated;
					material->color = (*item)->material.color;
					new_obj->AddComponent(material);
					break; }
				default:
					break;

				}
			}
		}
	}

	if (!obj->children.empty()) {
		std::vector<ActionDeleteObject*>::iterator item = obj->children.begin();
		for (; item != obj->children.end(); ++item) {
			if (*item != nullptr) {
				ReturnZ::CreateObject((*item));
			}
		}
	}


}
