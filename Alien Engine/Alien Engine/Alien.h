#pragma once

class ComponentTransform;
class GameObject;
enum class ComponentType;
class Component;

class __declspec(dllexport) Alien {
	friend class ComponentScript;
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

	bool HasComponent(const ComponentType& component);
	Component* GetComponent(const ComponentType& type);
	void* GetComponentScript(const char* script_class_name);
	Component* GetComponentInParent(const ComponentType& type);
	void* GetComponentScriptInParent(const char* script_class_name);
	Component* GetComponentInChildren(const ComponentType& type, bool recursive);

	void Destroy(GameObject* obj);

	bool CompareTag(const char* tag_to_compare);

	// Need
	// TODO:
	/*
		static GameObject* Clone(pos, parent...); // with clone of a gameobject

		change static, is static... cant move in code if is static!!

		GetComponents();
		GetComponentsInChildren();
		GetComponentsInParent();

		DestroyComponent();
		Destroyimmediate();
		DontDestroyOnLoad();
	*/


	// Extras
	// TODO: https://docs.unity3d.com/ScriptReference/MonoBehaviour.html
	/*
		SendMessage();
		SendMessageUpWards

		Invoke();
		CancelInvoke();
		InvokeRepeating();
		IsInvoking();

		StartCorrutine();
		CancelCorrutin();
		CancelAllCorrutines();

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

	// TODO:
	/*
		enabled
		game_object
		tag?????
		name
	*/
private:

	bool* enabled = nullptr;

};
