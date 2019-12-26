#pragma once

#include <functional>

class ComponentTransform;
class GameObject;
enum class ComponentType;
class Component;
typedef unsigned int uint;

class __declspec(dllexport) Alien {
	friend class ComponentScript;
	friend class ModuleObjects;
public:
	Alien();
	virtual ~Alien();

	virtual void Awake() {} 
	virtual void Start() {} 

	virtual void PreUpdate() {}
	virtual void Update() {}
	virtual void PostUpdate() {}

	virtual void OnDisable() {} 
	virtual void OnEnable() {} 

	virtual void OnDrawGizmos() {}
	virtual void OnDrawGizmosSelected() {}

	virtual void CleanUp() {}

	bool IsScriptEnabled() const;
	void SetScriptEnable(const bool& enable);

	const char* ToString();

	bool HasComponent(const ComponentType& component) const;
	Component* GetComponent(const ComponentType& type);
	const Component* GetComponent(const ComponentType& type) const;
	void* GetComponentScript(const char* script_class_name);
	const void* GetComponentScript(const char* script_class_name) const;
	Component* GetComponentInParent(const ComponentType& type);
	const Component* GetComponentInParent(const ComponentType& type) const;
	void* GetComponentScriptInParent(const char* script_class_name);
	const void* GetComponentScriptInParent(const char* script_class_name) const;
	Component* GetComponentInChildren(const ComponentType& type, bool recursive);
	const Component* GetComponentInChildren(const ComponentType& type, bool recursive) const;
	// return the sie of the array of components found, pass a Component** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	uint GetComponents(const ComponentType& type, Component*** comp_array);
	const uint GetComponents(const ComponentType& type, Component*** comp_array) const;
	// return the sie of the array of components found, pass a Component** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	uint GetComponentsInChildren(const ComponentType& type, Component*** comp_array, bool recursive);
	const uint GetComponentsInChildren(const ComponentType& type, Component*** comp_array, bool recursive) const;
	// return the sie of the array of components found, pass a Component** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	uint GetComponentsInParent(const ComponentType& type, Component*** comp_array);
	const uint GetComponentsInParent(const ComponentType& type, Component*** comp_array) const;
	// return the sie of the array of components found, pass a Component** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	uint GetComponentsScript(const char* script_class_name, void*** script_array);
	const uint GetComponentsScript(const char* script_class_name, void*** script_array) const;
	// return the sie of the array of components found, pass a ScriptClassToFind** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	uint GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive);
	const uint GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive) const;
	// return the sie of the array of components found, pass a Component** nullptr with &. Remember to delete it with GameObject::FreeArrayMemory!!!
	uint GetComponentsScriptInParent(const char* script_class_name, void*** script_array);
	const uint GetComponentsScriptInParent(const char* script_class_name, void*** script_array) const;

	static void Destroy(GameObject* obj);
	static void DestroyInstantly(GameObject* obj);

	bool CompareTag(const char* tag_to_compare) const;

	void Invoke(std::function<void()> void_no_params_function, float seconds);
	void CancelInvoke();
	void InvokeRepeating(std::function<void()> void_no_params_function, float seconds_to_first_invoke, float seconds_between_each_call);
	//bool IsInvoking(std::function<void()> void_no_params_function);

	// Need
	// TODO:
	/*
		change static, is static... cant move in code if is static!!

		DontDestroyOnLoad();
	*/


	// Extras
	// TODO: https://docs.unity3d.com/ScriptReference/MonoBehaviour.html
	/*
		SendMessage();
		SendMessageUpWards

		OnApplicationPause();
		OnApplicationResume();
		
		OnBecameInvisible();
		OnBecameVisible();

		OnGUI();

		OnMouseDown();
		OnMouseUp();
		OnMouseDrag();
		OnMouseEnter();
		OnMouseExit();
		OnMouseOver();

		OnPostRender(); // when a camera finished rendering
		OnRenderObject(); // when all cameras finished rendering
		OnPreCull();
		OnPreRender();
	*/

public:

	ComponentTransform* transform = nullptr;
	GameObject* game_object = nullptr;

private:

	bool* enabled = nullptr;
	char data_name[260];

};
