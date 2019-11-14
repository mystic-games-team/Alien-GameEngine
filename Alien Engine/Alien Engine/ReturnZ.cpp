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

	to_fill->object = new ObjZ();

	to_fill->object->enabled = obj->enabled;
	to_fill->object->is_static = obj->is_static;
	to_fill->object->ID = obj->ID;
	to_fill->object->parentID = obj->parent->ID;
	to_fill->object->parent_selected = obj->IsParentSelected();
	to_fill->object->selected = obj->IsSelected();
	to_fill->object->parent_enabled = obj->IsParentEnabled();
	to_fill->object->name = std::string(obj->GetName()).data();
	
	if (!obj->components.empty()) {
		std::vector<Component*>::iterator item = obj->components.begin();
		for (; item != obj->components.end(); ++item) {
			if (*item != nullptr) {
				CompZ* comp = nullptr;
				switch ((*item)->GetType()) {
				case ComponentType::TRANSFORM: {
					ComponentTransform* transform = (ComponentTransform*)obj->GetComponent(ComponentType::TRANSFORM);
					CompTransformZ* trans = new CompTransformZ();
					trans->pos = transform->GetLocalPosition();
					trans->scale = transform->GetLocalScale();
					trans->rot = transform->GetLocalRotation();
					trans->is_scale_negative = transform->IsScaleNegative();
					comp = trans;
					break; }
				case ComponentType::MESH: {
					ComponentMesh* mesh = (ComponentMesh*)obj->GetComponent(ComponentType::MESH);
					CompMeshZ* meshZ = new CompMeshZ();
					if (mesh->mesh != nullptr)
						meshZ->resourceID = mesh->mesh->GetID();
					meshZ->objectID = mesh->game_object_attached->ID;
					meshZ->draw_AABB = mesh->draw_AABB;
					meshZ->draw_OBB = mesh->draw_OBB;
					meshZ->view_face_normals = mesh->view_face_normals;
					meshZ->view_vertex_normals = mesh->view_vertex_normals;
					meshZ->wireframe = mesh->wireframe;
					meshZ->view_mesh = mesh->view_mesh;
					comp = meshZ;
					break; }
				case ComponentType::MATERIAL: {
					ComponentMaterial* material = (ComponentMaterial*)obj->GetComponent(ComponentType::MATERIAL);
					CompMaterialZ* materialZ = new CompMaterialZ();
					if (material->texture != nullptr)
						materialZ->resourceID = material->texture->GetID();
					materialZ->objectID = material->game_object_attached->ID;
					materialZ->color = material->color;
					materialZ->texture_activated = material->texture_activated;
					comp = materialZ;
					break; }
				}
				if (comp != nullptr) {
					comp->type = (*item)->GetType();
					to_fill->object->comps.push_back(comp);
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
				to_fill->object->children.push_back(obj);
			}
		}
	}
}

void ReturnZ::CreateObject(ActionDeleteObject* obj)
{
	GameObject* new_obj = new GameObject();
	new_obj->parent = App->objects->GetGameObjectByID(obj->object->parentID);
	if (new_obj->parent != nullptr) {
		new_obj->parent->AddChild(new_obj);
	}
	new_obj->enabled = obj->object->enabled;
	new_obj->is_static = obj->object->is_static;
	if (new_obj->is_static) {
		App->objects->octree.Insert(new_obj, false);
	}
	new_obj->ID = obj->object->ID;
	new_obj->SetName(obj->object->name.data());
	if (obj->object->selected) {
		App->objects->SetNewSelectedObject(new_obj);
	}
	new_obj->parent_enabled = obj->object->parent_enabled;
	new_obj->parent_selected = obj->object->parent_selected;

	if (!obj->object->comps.empty()) {
		std::vector<CompZ*>::iterator item = obj->object->comps.begin();
		for (; item != obj->object->comps.end(); ++item) {
			if (*item != nullptr) {
				SDL_assert((uint)ComponentType::UNKNOWN == 4); // add new type to switch
				switch ((*item)->type)
				{
				case ComponentType::TRANSFORM: {
					CompTransformZ* trans = (CompTransformZ*)(*item);
					new_obj->AddComponent(new ComponentTransform(new_obj, trans->pos, trans->rot, trans->scale));
					static_cast<ComponentTransform*>(new_obj->components.back())->is_scale_negative = trans->is_scale_negative;
					break; }
				case ComponentType::MESH: {
					ComponentMesh* mesh = new ComponentMesh(new_obj);
					CompMeshZ* meshZ = (CompMeshZ*)(*item);
					if (meshZ->resourceID != 0)
						mesh->mesh = (ResourceMesh*)App->resources->GetResourceWithID(meshZ->resourceID);
					mesh->draw_AABB = meshZ->draw_AABB;
					mesh->draw_OBB = meshZ->draw_OBB;
					mesh->wireframe = meshZ->wireframe;
					mesh->view_mesh = meshZ->view_mesh;
					mesh->view_face_normals = meshZ->view_face_normals;
					mesh->view_vertex_normals = meshZ->view_vertex_normals;
					mesh->RecalculateAABB_OBB();
					new_obj->AddComponent(mesh);
					break; }
				case ComponentType::MATERIAL: {
					ComponentMaterial* material = new ComponentMaterial(new_obj);
					CompMaterialZ* materialZ = (CompMaterialZ*)(*item);
					if (materialZ->resourceID != 0)
						material->texture = (ResourceTexture*)App->resources->GetResourceWithID(materialZ->resourceID);
					material->texture_activated = materialZ->texture_activated;
					material->color = materialZ->color;
					new_obj->AddComponent(material);
					break; }
				default:
					break;

				}
			}
		}
	}
	
	if (!obj->object->children.empty()) {
		std::vector<ActionDeleteObject*>::iterator item = obj->object->children.begin();
		for (; item != obj->object->children.end(); ++item) {
			if (*item != nullptr) {
				ReturnZ::CreateObject((*item));
			}
		}
	}


}
