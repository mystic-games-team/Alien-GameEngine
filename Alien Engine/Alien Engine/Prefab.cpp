#include "Prefab.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ResourcePrefab.h"


Prefab::Prefab()
{
}

Prefab::~Prefab()
{
}

GameObject* Prefab::ConvertToGameObject(float3 local_position, GameObject* parent)
{
	if (prefabID == 0) {
		LOG("Prefab is NULL or might not exist");
		return nullptr;
	}
	ResourcePrefab* prefab = (ResourcePrefab*)App->resources->GetResourceWithID(prefabID);
	if (prefab == nullptr) {
		LOG("Prefab is NULL or might not exist");
		return nullptr;
	}
	prefab->ConvertToGameObjects((parent == nullptr) ? App->objects->GetRoot(true) : parent, -1, local_position, false);
}
