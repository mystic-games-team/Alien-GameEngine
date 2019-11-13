#include "ReturnZ.h"
#include "Application.h"
#include "ModuleObjects.h"

void ReturnZ::SetAction(const ReturnActions& type, void* data)
{
	action.type = type;

	switch (type)
	{
	case ReturnActions::DELETE_OBJECT: {
		SetDeleteObject((GameObject*)data);
		break; }
	default:
		break;
	}
}

void ReturnZ::AddNewAction(const ReturnActions& type, void* data)
{
	ReturnZ ret;
	ret.SetAction(type, data);
	App->objects->return_actions.push(ret);
}

ReturnZ::Actions::Obj ReturnZ::SetDeleteObject(GameObject* obj)
{
	action.object.enabled = obj->enabled;
	action.object.is_static = obj->is_static;
	action.object.ID = obj->ID;
	action.object.parentID = obj->parent->ID;
	action.object.parent_selected = obj->IsParentSelected();
	action.object.selected = obj->IsSelected();
	action.object.parent_enabled = obj->IsParentEnabled();
	action.object.name = std::string(obj->GetName());
	// components



	if (!obj->children.empty()) {
		std::vector<GameObject*>::iterator item = obj->children.begin();
		for (; item != obj->children.end(); ++item) {
			if (*item != nullptr) {
				action.object.children.push_back(&(SetDeleteObject(*item)));
			}
		}
	}

	return action.object;
}
