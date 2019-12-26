#include "Alien.h"
#include "Application.h"
#include "GameObject.h"

Alien::Alien()
{
}

Alien::~Alien()
{
	App->objects->CancelInvokes(this);
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

bool Alien::HasComponent(const ComponentType& component) const
{
	return game_object->HasComponent(component);
}

Component* Alien::GetComponent(const ComponentType& type)
{
	return game_object->GetComponent(type);
}

const Component* Alien::GetComponent(const ComponentType& type) const
{
	return game_object->GetComponent(type);
}

void* Alien::GetComponentScript(const char* script_class_name)
{
	return game_object->GetComponentScript(script_class_name);
}

const void* Alien::GetComponentScript(const char* script_class_name) const
{
	return game_object->GetComponentScript(script_class_name);
}

Component* Alien::GetComponentInParent(const ComponentType& type)
{
	return game_object->GetComponentInParent(type);
}

const Component* Alien::GetComponentInParent(const ComponentType& type) const
{
	return game_object->GetComponentInParent(type);
}

void* Alien::GetComponentScriptInParent(const char* script_class_name)
{
	return game_object->GetComponentScriptInParent(script_class_name);
}

const void* Alien::GetComponentScriptInParent(const char* script_class_name) const
{
	return game_object->GetComponentScriptInParent(script_class_name);
}

Component* Alien::GetComponentInChildren(const ComponentType& type, bool recursive)
{
	return game_object->GetComponentInChildren(type, recursive);
}

const Component* Alien::GetComponentInChildren(const ComponentType& type, bool recursive) const
{
	return game_object->GetComponentInChildren(type, recursive);
}

uint Alien::GetComponents(const ComponentType& type, Component*** comp_array)
{
	return game_object->GetComponents(type, comp_array);
}

const uint Alien::GetComponents(const ComponentType& type, Component*** comp_array) const
{
	return game_object->GetComponents(type, comp_array);
}

uint Alien::GetComponentsInChildren(const ComponentType& type, Component*** comp_array, bool recursive)
{
	return game_object->GetComponentsInChildren(type, comp_array, recursive);
}

const uint Alien::GetComponentsInChildren(const ComponentType& type, Component*** comp_array, bool recursive) const
{
	return game_object->GetComponentsInChildren(type, comp_array, recursive);
}

uint Alien::GetComponentsInParent(const ComponentType& type, Component*** comp_array)
{
	return game_object->GetComponentsInParent(type, comp_array);
}

const uint Alien::GetComponentsInParent(const ComponentType& type, Component*** comp_array) const
{
	return game_object->GetComponentsInParent(type, comp_array);
}

uint Alien::GetComponentsScript(const char* script_class_name, void*** script_array)
{
	return game_object->GetComponentsScript(script_class_name, script_array);
}

const uint Alien::GetComponentsScript(const char* script_class_name, void*** script_array) const
{
	return game_object->GetComponentsScript(script_class_name, script_array);
}

uint Alien::GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive)
{
	return game_object->GetComponentsScriptInChildren(script_class_name, script_array, recursive);
}

const uint Alien::GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive) const
{
	return game_object->GetComponentsScriptInChildren(script_class_name, script_array, recursive);
}

uint Alien::GetComponentsScriptInParent(const char* script_class_name, void*** script_array)
{
	return game_object->GetComponentsScriptInParent(script_class_name, script_array);
}

const uint Alien::GetComponentsScriptInParent(const char* script_class_name, void*** script_array) const
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

bool Alien::CompareTag(const char* tag_to_compare) const
{
	return strcmp(game_object->GetTag(), tag_to_compare) == 0;
}

void Alien::Invoke(std::function<void()> void_no_params_function, float seconds)
{
	if (void_no_params_function != nullptr) {
		App->objects->AddInvoke(void_no_params_function, seconds, this);
	}
}

void Alien::CancelInvoke()
{
	App->objects->CancelInvokes(this);
}

void Alien::InvokeRepeating(std::function<void()> void_no_params_function, float seconds_to_first_invoke, float seconds_between_each_call)
{
	if (void_no_params_function != nullptr) {
		App->objects->AddInvokeRepeating(void_no_params_function, seconds_to_first_invoke, seconds_between_each_call, this);
	}
}

//bool Alien::IsInvoking(std::function<void()> void_no_params_function)
//{
//	return App->objects->IsInvoking(void_no_params_function);
//}

