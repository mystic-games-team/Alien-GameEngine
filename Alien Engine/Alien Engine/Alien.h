#pragma once

class ComponentTransform;
class GameObject;

class __declspec(dllexport) Alien {
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

	virtual void CleanUp() {}
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

		OnDrawGizmos();
		OnDrawGizmosSelected();

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

};
