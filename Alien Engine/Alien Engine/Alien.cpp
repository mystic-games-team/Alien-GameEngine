#include "Alien.h"
#include "GameObject.h"

Alien::Alien()
{
}

Alien::~Alien()
{
}

bool Alien::IsScriptEnabled() const
{
	return (enabled == nullptr) ? false : *enabled;
}

void Alien::SetScriptEnable(const bool& enable)
{
	if (enable != *enabled) {
		*enabled = enable;
		if (enable) {
			OnEnable();
		}
		else {
			OnDisable();
		}
	}
}

const char* Alien::ToString()
{
	return game_object->ToString();
}

bool Alien::HasComponent(const ComponentType& component)
{
	return game_object->HasComponent(component);
}

Component* Alien::GetComponent(const ComponentType& type)
{
	return game_object->GetComponent(type);
}

void* Alien::GetComponentScript(const char* script_class_name)
{
	return game_object->GetComponentScript(script_class_name);
}

Component* Alien::GetComponentInParent(const ComponentType& type)
{
	return game_object->GetComponentInParent(type);
}

void* Alien::GetComponentScriptInParent(const char* script_class_name)
{
	return game_object->GetComponentScriptInParent(script_class_name);
}

Component* Alien::GetComponentInChildren(const ComponentType& type, bool recursive)
{
	return game_object->GetComponentInChildren(type, recursive);
}

void Alien::Destroy(GameObject* obj)
{
	GameObject::Destroy(obj);
}
