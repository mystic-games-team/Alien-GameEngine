#include "ReturnZ.h"
#include "Application.h"
#include "ModuleObjects.h"
#include "ComponentTransform.h"

void ReturnZ::SetAction(const ReturnActions& type, void* data)
{
	this->type = type;

	switch (type)
	{
	case ReturnActions::DELETE_OBJECT: {
		SetDeleteObject((GameObject*)data, object);
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
		to_return->CreateObject(to_return->object);
		break; }
	}

	App->objects->return_actions.pop();
}

ReturnZ::Obj ReturnZ::SetDeleteObject(GameObject* obj, Obj& to_fill)
{
	to_fill.enabled = obj->enabled;
	to_fill.is_static = obj->is_static;
	to_fill.ID = obj->ID;
	to_fill.parentID = obj->parent->ID;
	to_fill.parent_selected = obj->IsParentSelected();
	to_fill.selected = obj->IsSelected();
	to_fill.parent_enabled = obj->IsParentEnabled();
	to_fill.name = std::string(obj->GetName()).data();
	// components
	
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
					to_fill.comps.push_back(comp);
					break; }
				}
			}
		}
	}

	if (!obj->children.empty()) {
		std::vector<GameObject*>::iterator item = obj->children.begin();
		for (; item != obj->children.end(); ++item) {
			if (*item != nullptr) {
				Obj obj;
				to_fill.children.push_back(&(SetDeleteObject((*item), obj)));
			}
		}
	}

	return object;
}

void ReturnZ::CreateObject(Obj obj)
{
	GameObject* new_obj = new GameObject();
	new_obj->parent = App->objects->GetGameObjectByID(obj.parentID);
	if (new_obj->parent != nullptr) {
		new_obj->parent->AddChild(new_obj);
	}
	new_obj->enabled = obj.enabled;
	new_obj->is_static = obj.is_static;
	if (new_obj->is_static) {
		App->objects->octree.Insert(new_obj, false);
	}
	new_obj->ID = obj.ID;
	new_obj->SetName(obj.name.data());
	if (obj.selected) {
		App->objects->SetNewSelectedObject(new_obj);
	}
	new_obj->parent_enabled = obj.parent_enabled;
	new_obj->parent_selected = obj.parent_selected;

	if (!obj.comps.empty()) {
		std::vector<Obj::Comp*>::iterator item = obj.comps.begin();
		for (; item != obj.comps.end(); ++item) {
			if (*item != nullptr) {
				switch ((*item)->type)
				{
				case ComponentType::TRANSFORM: {
					new_obj->AddComponent(new ComponentTransform(new_obj, (*item)->transform.pos, (*item)->transform.rot, (*item)->transform.scale));
					break; }
				default:
					break;

				}
			}
		}
	}


}
