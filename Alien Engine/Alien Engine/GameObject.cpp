#include "Color.h"
#include "GameObject.h"
#include "glew/include/glew.h"
#include "Application.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentLight.h"
#include "RandomHelper.h"
#include "ModuleObjects.h"
#include "ComponentCamera.h"
#include "ComponentScript.h"
#include "ReturnZ.h"

GameObject::GameObject(GameObject* parent)
{
	ID = App->resources->GetRandomID();

	if (parent != nullptr) {
		this->parent = parent;
		parent->AddChild(this);
	}
}

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	if (App->objects->GetSelectedObject() == this)
		App->objects->DeselectObject();

	App->objects->octree.Remove(this);

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

void GameObject::SetEnable(bool enable)
{
	enabled = enable;
}

bool GameObject::IsEnabled()
{
	return enabled;
}

void GameObject::DrawScene()
{
	ComponentTransform* transform = (ComponentTransform*)GetComponent(ComponentType::TRANSFORM);
	ComponentMaterial* material = (ComponentMaterial*)GetComponent(ComponentType::MATERIAL);
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);

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
		if (mesh->draw_AABB)
			mesh->DrawGlobalAABB();
		if (mesh->draw_OBB)
			mesh->DrawOBB();
	}
}


void GameObject::DrawGame()
{
	ComponentMaterial* material = (ComponentMaterial*)GetComponent(ComponentType::MATERIAL);
	
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);

	if (material != nullptr && material->IsEnabled() && mesh != nullptr && mesh->IsEnabled())
	{
		material->BindTexture();
	}

	if (mesh != nullptr && mesh->IsEnabled())
	{
		if (material == nullptr || (material != nullptr && !material->IsEnabled())) // set the basic color if the GameObject hasn't a material
			glColor3f(1, 1, 1);
		mesh->DrawPolygon();
	}
}

void GameObject::SetDrawList(std::vector<std::pair<float, GameObject*>>* to_draw, const ComponentCamera* camera)
{
	if (!is_static) {
		ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);

		if (mesh != nullptr && mesh->mesh != nullptr) {
			if (App->renderer3D->IsInsideFrustum(camera, mesh->GetGlobalAABB())) {
				float3 obj_pos = static_cast<ComponentTransform*>(GetComponent(ComponentType::TRANSFORM))->GetGlobalPosition();
				float distance = camera->frustum.pos.Distance(obj_pos);
				to_draw->push_back({ distance, this });
			}
		}
	}

	ComponentLight* light = (ComponentLight*)GetComponent(ComponentType::LIGHT);
	if (light != nullptr && light->IsEnabled())
	{
		light->LightLogic();
	}
	ComponentTransform* transform = (ComponentTransform*)GetComponent(ComponentType::TRANSFORM);
	ComponentCamera* camera_ = (ComponentCamera*)GetComponent(ComponentType::CAMERA);
	if (camera_ != nullptr && camera_->IsEnabled() && App->objects->GetSelectedObject() == this) 
	{
		if (App->objects->printing_scene && App->objects->draw_frustum)
			camera_->DrawFrustum();
		camera_->frustum.pos = transform->GetGlobalPosition();
		camera_->frustum.front = transform->GetGlobalRotation().WorldZ();
		camera_->frustum.up = transform->GetGlobalRotation().WorldY();
	}

	if (App->objects->printing_scene)
	{
		if (camera_ != nullptr && camera_->IsEnabled())
		{
			camera_->DrawIconCamera();
		}

		if (light != nullptr && light->IsEnabled())
		{
			light->DrawIconLight();
		}
	}
	std::vector<GameObject*>::iterator child = children.begin();
	for (; child != children.end(); ++child) {
		if (*child != nullptr && (*child)->IsEnabled()) {
			(*child)->SetDrawList(to_draw, camera);
		}
	}
}

void GameObject::AddComponent(Component* component)
{
	for (auto item = components.begin(); item != components.end(); ++item) {
		if (*item != nullptr && *item == component) {
			return;
		}
	}
	components.push_back(component);
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
	strcpy(this->name, name);
}

const char* GameObject::GetName()
{
	return name;
}

void GameObject::SetTag(const char* tag)
{
	strcpy(this->tag, tag);
}

const char* GameObject::GetTag()
{
	return tag;
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

Component* GameObject::GetComponentWithID(const u64& compID)
{
	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item) {
		if (*item != nullptr && (*item)->ID == compID) {
			return *item;
		}
	}
	return nullptr;
}

void GameObject::RemoveComponent(Component* component)
{
	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item) {
		if (*item != nullptr && *item == component) {
			delete* item;
			(*item) = nullptr;
			components.erase(item);
			break;
		}
	}
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

void GameObject::ChangeAABB(const bool& AABB_view)
{
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);

	if (mesh != nullptr)
		mesh->draw_AABB = AABB_view;

	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->ChangeAABB(AABB_view);
		}
	}
}

void GameObject::ChangeOBB(const bool& OBB_view)
{
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);

	if (mesh != nullptr)
		mesh->draw_OBB = OBB_view;

	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->ChangeOBB(OBB_view);
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
		parent->children.erase(std::find(parent->children.begin(), parent->children.end(), this));
		parent = new_parent;
		parent->AddChild(this);

		ComponentTransform* parent_transform = (ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM);
		ComponentTransform* transform = (ComponentTransform*)GetComponent(ComponentType::TRANSFORM);

		if (parent_transform != nullptr) {
			transform->Reparent(parent_transform->global_transformation.Inverted() * transform->global_transformation);
		}
		else {
			transform->Reparent(transform->global_transformation);
		}

	}
}

void GameObject::ToDelete()
{
	to_delete = true;
	App->objects->need_to_delete_objects = true;
	if (!App->objects->in_cntrl_Z)
		ReturnZ::AddNewAction(ReturnZ::ReturnActions::DELETE_OBJECT, App->objects->GetSelectedObject());
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

void GameObject::Destroy(GameObject* object)
{
	object->ToDelete();
}

GameObject* GameObject::FindWithName(const char* name)
{
	return App->objects->GetRoot(true)->Find(name);
}

GameObject* GameObject::FindWithTag(const char* tag_to_find)
{
	return App->objects->GetRoot(true)->FindTag(tag_to_find);
}

uint GameObject::FindGameObjectsWithTag(const char* tag_to_find, GameObject*** objects)
{
	std::vector<GameObject*> found;
	App->objects->GetRoot(true)->FindTags(tag_to_find, &found);

	if (found.size() > 0) {
		(*objects) = new GameObject*[found.size()];
		for (uint i = 0; i < found.size(); ++i) {
			(*objects)[i] = found[i];
		}
	}

	return found.size();
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

GameObject* GameObject::Find(const char* name)
{
	GameObject* ret = nullptr;
	if (App->StringCmp(name, this->name)) {
		return this;
	}
	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			ret = (*item)->Find(name);
			if (ret != nullptr)
				break;
		}
	}
	return ret;
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

void GameObject::ReTag(const char* from, const char* to)
{
	if (App->StringCmp(tag, from)) {
		strcpy(tag, to);
	}
	for (uint i = 0; i < children.size(); ++i) {
		if (children[i] != nullptr) {
			children[i]->ReTag(from, to);
		}
	}
}

GameObject* GameObject::GetGameObjectByID(const u64 & id)
{
	GameObject* ret = nullptr;
	if (id == this->ID) {
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

GameObject* GameObject::FindTag(const char* tag_to_find)
{
	GameObject* ret = nullptr;
	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			if (App->StringCmp((*item)->tag, tag_to_find)) {
				return this;
			}
			ret = (*item)->FindTag(tag_to_find);
			if (ret != nullptr)
				break;
		}
	}
	return ret;
}

void GameObject::FindTags(const char* tag_to_find, std::vector<GameObject*>* objects)
{
	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			if (App->StringCmp((*item)->tag, tag_to_find)) {
				objects->push_back(this);
			}
			(*item)->FindTags(tag_to_find, objects);
		}
	}
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

AABB GameObject::GetBB()
{
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);

	if (HasChildren())
	{
		AABB parent_aabb;
		if (mesh != nullptr)
		{
			parent_aabb = mesh->GetGlobalAABB();
		}
		else
		{
			parent_aabb.SetNegativeInfinity();
		}

		for (std::vector<GameObject*>::iterator iter = children.begin(); iter != children.end(); ++iter)
		{
			AABB child_aabb = (*iter)->GetBB();
			parent_aabb.maxPoint = parent_aabb.maxPoint.Max(child_aabb.maxPoint);
			parent_aabb.minPoint = parent_aabb.minPoint.Min(child_aabb.minPoint);
		}

		return parent_aabb;
	}

	else
	{
		if (mesh != nullptr)
		{
			return mesh->GetGlobalAABB();
		}
		else
		{
			ComponentCamera* camera = (ComponentCamera*)GetComponent(ComponentType::CAMERA);
			ComponentLight* light = (ComponentLight*)GetComponent(ComponentType::LIGHT);

			if (camera != nullptr) {
				ComponentTransform* transform = (ComponentTransform*)GetComponent(ComponentType::TRANSFORM);
				float3 pos = transform->GetLocalPosition();
				Quat rot = transform->GetLocalRotation();
				float3 scale = transform->GetLocalScale();
				transform->SetLocalScale(0.1f, 0.1f, 0.1f);
				float3 position = pos - camera->frustum.front.Normalized() * 2;
				Quat right_rot = { 0.7071,0,0.7071,0 };
				Quat rotation = { 0,0,1,0 };
				Quat rotated = rot * (rotation * right_rot);
				transform->SetLocalPosition(position.x, position.y, position.z);
				transform->SetLocalRotation(rotated.x, rotated.y, rotated.z, rotated.w);
				camera->mesh_camera->RecalculateAABB_OBB();
				transform->SetLocalScale(scale.x, scale.y, scale.z);
				transform->SetLocalPosition(pos.x, pos.y, pos.z);
				transform->SetLocalRotation(rot.x, rot.y, rot.z, rot.w);
				return camera->mesh_camera->GetGlobalAABB();
			}
			else if (light != nullptr) {
				ComponentTransform* transform = (ComponentTransform*)GetComponent(ComponentType::TRANSFORM);
				float3 pos = transform->GetLocalPosition();
				float3 scale = transform->GetLocalScale();
				transform->SetLocalScale(0.2f, 0.18f, 0.2f);
				transform->SetLocalPosition(pos.x - 0.133f, pos.y, pos.z);
				light->bulb->RecalculateAABB_OBB();
				transform->SetLocalScale(scale.x, scale.y, scale.z);
				transform->SetLocalPosition(pos.x, pos.y, pos.z);
				return light->bulb->GetGlobalAABB();
			}

			AABB aabb_null;
			ComponentTransform* transform = (ComponentTransform*)GetComponent(ComponentType::TRANSFORM);
			float3 pos = transform->GetGlobalPosition();
			aabb_null.SetFromCenterAndSize(pos, { 2,2,2 });
			return aabb_null;
		}
	}
}

OBB GameObject::GetGlobalOBB()
{
	ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);
	ComponentTransform* transform = (ComponentTransform*)GetComponent(ComponentType::TRANSFORM);

	if (HasChildren())
	{
		OBB parent_obb = GetBB();
		parent_obb.Transform(transform->global_transformation);
		return parent_obb;
	}

	else
	{
		if (mesh != nullptr)
		{
			return mesh->GetOBB();
		}
		else
		{
			AABB aabb_null;
			ComponentTransform* transform = (ComponentTransform*)GetComponent(ComponentType::TRANSFORM);
			float3 pos = transform->GetGlobalPosition();
			aabb_null.SetFromCenterAndSize(pos, { 2,2,2 });
			return aabb_null;
		}
	}
}

void GameObject::SaveObject(JSONArraypack* to_save, const uint& family_number)
{
	to_save->SetString("Name", name);
	to_save->SetNumber("FamilyNumber", family_number);
	to_save->SetString("ID", std::to_string(ID));
	to_save->SetString("ParentID",(parent != nullptr) ? std::to_string(parent->ID) : "0");
	to_save->SetBoolean("Enabled", enabled);
	to_save->SetBoolean("ParentEnabled", parent_enabled);
	to_save->SetBoolean("Selected", selected);
	to_save->SetBoolean("ParentSelected", parent_selected);
	to_save->SetBoolean("IsStatic", is_static);
	to_save->SetBoolean("IsPrefab", IsPrefab());
	to_save->SetBoolean("PrefabLocked", prefab_locked);
	to_save->SetString("Tag", tag);
	if (IsPrefab()) {
		to_save->SetString("PrefabID", std::to_string(prefabID));
	}

	JSONArraypack* components_to_save = to_save->InitNewArray("Components");

	std::vector<Component*>::iterator item = components.begin();
	for (; item != components.end(); ++item) {
		if (*item != nullptr) {
			(*item)->SaveComponent(components_to_save);
			if ((*item) != components.back())
				components_to_save->SetAnotherNode();
		}
	}
}

void GameObject::LoadObject(JSONArraypack* to_load, GameObject* parent)
{
	strcpy(name, to_load->GetString("Name"));
	ID = std::stoull(to_load->GetString("ID"));
	enabled = to_load->GetBoolean("Enabled");
	parent_enabled = to_load->GetBoolean("ParentEnabled");
	if (to_load->GetBoolean("Selected")) {
		App->objects->SetNewSelectedObject(this);
	}
	prefab_locked = to_load->GetBoolean("PrefabLocked");
	parent_selected = to_load->GetBoolean("ParentSelected");
	is_static = to_load->GetBoolean("IsStatic");
	std::string tag_ = to_load->GetString("Tag");
	if (std::find(App->objects->tags.begin(), App->objects->tags.end(), tag_) != App->objects->tags.end()) {
		strcpy(tag, tag_.data());
	}
	if (to_load->GetBoolean("IsPrefab")) {
		u64 id = std::stoull(to_load->GetString("PrefabID"));
		if (App->resources->GetResourceWithID(id) != nullptr) {
			prefabID = id;
		}
	}
	if (parent != nullptr) {
		this->parent = parent;
		parent->AddChild(this);
	}

	JSONArraypack* components_to_load = to_load->GetArray("Components");

	if (components_to_load != nullptr) {
		for (uint i = 0; i < components_to_load->GetArraySize(); ++i) {
			SDL_assert((uint)ComponentType::UNKNOWN == 4); // add new type to switch
			switch ((int)components_to_load->GetNumber("Type")) {
			case (int)ComponentType::TRANSFORM: {
				ComponentTransform* transform = new ComponentTransform(this);
				transform->LoadComponent(components_to_load);
				AddComponent(transform);
				break; }
			case (int)ComponentType::LIGHT: {
				ComponentLight* light = new ComponentLight(this);
				light->LoadComponent(components_to_load);
				AddComponent(light);
				break; }
			case (int)ComponentType::MATERIAL: {
				ComponentMaterial* material = new ComponentMaterial(this);
				material->LoadComponent(components_to_load);
				AddComponent(material);
				break; }
			case (int)ComponentType::MESH: {
				ComponentMesh* mesh = new ComponentMesh(this);
				mesh->LoadComponent(components_to_load);
				AddComponent(mesh);
				break; }
			case (int)ComponentType::CAMERA: {
				ComponentCamera* camera = new ComponentCamera(this);
				camera->LoadComponent(components_to_load);
				AddComponent(camera);
				break; }
			case (int)ComponentType::SCRIPT: {
				ComponentScript* script = new ComponentScript(this);
				script->LoadComponent(components_to_load);
				// dont need to addcomponent, load script does it
				break; }
			default:
				LOG("Unknown component type while loading");
				break;
			}

			components_to_load->GetAnotherNode();
		}
	}

	if (is_static) {
		App->objects->octree.Insert(this, false);
	}

}

void GameObject::SearchResourceToDelete(const ResourceType& type, Resource* to_delete)
{
	SDL_assert((uint)FileDropType::UNKNOWN == 5);
	switch (type) {
	case ResourceType::RESOURCE_TEXTURE: {
		ComponentMaterial* material = (ComponentMaterial*)GetComponent(ComponentType::MATERIAL);
		if (material != nullptr && material->GetTexture() == (ResourceTexture*)to_delete) {
			material->SetTexture(nullptr);
		}
		break; }
	case ResourceType::RESOURCE_MESH: {
		ComponentMesh* mesh = (ComponentMesh*)GetComponent(ComponentType::MESH);
		if (mesh != nullptr && mesh->mesh == (ResourceMesh*)to_delete) {
			mesh->mesh = nullptr;
		}
		break; }
	}

	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->SearchResourceToDelete(type, to_delete);
		}
	}
}

void GameObject::SetPrefab(const u64& prefabID)
{
	if (!children.empty()) {
		std::vector<GameObject*>::iterator item = children.begin();
		for (; item != children.end(); ++item) {
			if (*item != nullptr && (prefabID != 0 || (*item)->prefabID == this->prefabID)) {
				(*item)->SetPrefab(prefabID);
			}
		}
	}
	this->prefabID = prefabID;
}

void GameObject::UnpackPrefab()
{
	if (!IsPrefab())
		return;

	SetPrefab(0);
}

void GameObject::UnpackAllPrefabsOf(const u64& prefabID)
{
	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			if ((*item)->prefabID == prefabID) {
				(*item)->prefabID = 0;
			}
			(*item)->UnpackAllPrefabsOf(prefabID);
		}
	}
}

GameObject* GameObject::FindPrefabRoot()
{
	if (!IsPrefab())
		return nullptr;

	if (parent->IsPrefab()) {
		GameObject* find_root = this;
		for (;;) {
			if (find_root->parent != nullptr && (!find_root->parent->IsPrefab() || find_root->parent->prefabID != find_root->prefabID)) {
				return find_root;
			}
			find_root = find_root->parent;
		}
	}
	else {
		return this;
	}
}

bool GameObject::IsPrefab() const
{
	return prefabID != 0;
}

const u64 GameObject::GetPrefabID() const
{
	return prefabID;
}

void GameObject::GetObjectWithPrefabID(const u64& prefID, std::vector<GameObject*>* objs)
{
	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			if ((*item)->IsPrefab() && (*item)->prefabID == prefID && (*item)->FindPrefabRoot() == (*item)) {
				objs->push_back((*item));
			}
			else
				(*item)->GetObjectWithPrefabID(prefID, objs);
		}
	}
}

void GameObject::LockPrefab(bool lock)
{
	if (!IsPrefab())
		return;

	prefab_locked = lock;
	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->LockPrefab(lock);
		}
	}
}

void GameObject::ResetIDs()
{
	ID = App->resources->GetRandomID();

	if (!components.empty()) {
		std::vector<Component*>::iterator item = components.begin();
		for (; item != components.end(); ++item) {
			if (*item != nullptr) {
				(*item)->ResetIDs();
			}
		}
	}
	if (!children.empty()) {
		std::vector<GameObject*>::iterator item = children.begin();
		for (; item != children.end(); ++item) {
			if (*item != nullptr) {
				(*item)->ResetIDs();
			}
		}
	}
}

void GameObject::ChangeStatic(bool static_)
{
	std::vector<GameObject*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			(*item)->is_static = static_;
			(*item)->ChangeStatic(static_);
		}
	}
}

bool GameObject::HasChildrenStatic() const 
{
	bool ret = false;

	if (!children.empty()) {
		std::vector<GameObject*>::const_iterator item = children.cbegin();
		for (; item != children.cend(); ++item) {
			if (*item != nullptr) {
				if (ret)
					break;

				if ((*item)->is_static) {
					ret = true;
					break;
				}
				else {
					ret = (*item)->HasChildrenStatic();
				}
			}
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






