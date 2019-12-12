#pragma once

class ComponentTransform;
class GameObject;

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

	virtual void OnDisable() {} // TODO
	virtual void OnEnable() {} // TODO

	virtual void OnDrawGizmos() {}
	virtual void OnDrawGizmosSelected() {}

	virtual void CleanUp() {}

	bool IsScriptEnabled() const;
	void SetScriptEnable(const bool& enable);

	// Need
	// TODO:
	/*
		GetComponent();
		GetComponentInChildren();
		GetComponentInParent();
		GetComponents();
		GetComponentsInChildren();
		GetComponentsInParent();
		TryGetComponent();
		
		AddComponent(); ???????????????

		GetInstanceID();

		ToString(); // returns gameobject name

		Destroy();
		Destroyimmediate();
		DestroyComponent();
		DontDestroyOnLoad();

		Function to create prefab or someting xd
		
	*/


	// Extras
	// TODO: https://docs.unity3d.com/ScriptReference/MonoBehaviour.html
	/*
		
		OnValidate(); ??

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

		OnDestroy();

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
		
		CompareTag();
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
