#include "Prefab.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ResourcePrefab.h"
#include "Alien.h"
#include "ComponentScript.h"

Prefab::Prefab()
{
}

Prefab::~Prefab()
{
	if (prefabID != 0) {
		ResourcePrefab* prefab = (ResourcePrefab*)App->resources->GetResourceWithID(prefabID);
		if (prefab != nullptr) {
			prefab->prefab_references.remove(this);
		}
	}
}

GameObject* Prefab::ConvertToGameObject(float3 local_position, GameObject* parent)
{
	if (prefabID == 0) {
		LOG_ENGINE("Prefab is NULL or might not exist");
		return nullptr;
	}
	ResourcePrefab* prefab = (ResourcePrefab*)App->resources->GetResourceWithID(prefabID);
	if (prefab == nullptr) {
		LOG_ENGINE("Prefab is NULL or might not exist");
		return nullptr;
	}
	prefab->ConvertToGameObjects((parent == nullptr) ? App->objects->GetRoot(true) : parent, -1, local_position, false);

	GameObject* created = (parent == nullptr) ? App->objects->GetRoot(true)->children.back() : parent->children.back();

	InitScripts(created);

	return created;
}

void Prefab::InitScripts(GameObject* obj)
{
	if (!obj->components.empty()) {
		auto item = obj->components.begin();
		for (; item != obj->components.end(); ++item) {
			if (*item != nullptr && (*item)->GetType() == ComponentType::SCRIPT) {
				ComponentScript* script = (ComponentScript*)*item;
				if (script->need_alien && script->data_ptr != nullptr) {
					Alien* alien = (Alien*)script->data_ptr;
					if (alien != nullptr) {
						alien->Awake();
						alien->Start();
					}
				}
			}
		}
	}

	if (!obj->children.empty()) {
		auto item = obj->children.begin();
		for (; item != obj->children.end(); ++item) {
			if ((*item) != nullptr) {
				InitScripts(*item);
			}
		}
	}
}
