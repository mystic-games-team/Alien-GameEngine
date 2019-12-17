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

uint Alien::GetComponents(const ComponentType& type, Component*** comp_array)
{
	return game_object->GetComponents(type, comp_array);
}

uint Alien::GetComponentsInChildren(const ComponentType& type, Component*** comp_array, bool recursive)
{
	return game_object->GetComponentsInChildren(type, comp_array, recursive);
}

uint Alien::GetComponentsInParent(const ComponentType& type, Component*** comp_array)
{
	return game_object->GetComponentsInParent(type, comp_array);
}

uint Alien::GetComponentsScript(const char* script_class_name, void*** script_array)
{
	return game_object->GetComponentsScript(script_class_name, script_array);
}

uint Alien::GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive)
{
	return game_object->GetComponentsScriptInChildren(script_class_name, script_array, recursive);
}

uint Alien::GetComponentsScriptInParent(const char* script_class_name, void*** script_array)
{
	return game_object->GetComponentsScriptInParent(script_class_name, script_array);
}

void Alien::Destroy(GameObject* obj)
{
	GameObject::Destroy(obj);
}

void Alien::DestroyInstantly(GameObject* obj)
{
	GameObject::DestroyInstantly(obj);
}

bool Alien::CompareTag(const char* tag_to_compare)
{
	return strcmp(game_object->GetTag(), tag_to_compare) == 0;
}
