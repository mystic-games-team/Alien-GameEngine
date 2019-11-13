#pragma once 

#include "ModuleObjects.h"



class ReturnZ {

public:

	enum class ReturnActions {
		UNKNOWKN = -1,

		DELETE_OBJECT,
		ADD_OBJECT,
	};

	ReturnZ() {}
	~ReturnZ() {}

	static void AddNewAction(const ReturnActions& type, void* data);
	static void GoBackOneAction();

public:

	ReturnActions type = ReturnActions::UNKNOWKN;

	struct Obj {

		bool enabled = false;
		bool is_static = false;
		u64 ID = 0;
		u64 parentID = 0;
		bool to_delete = false;
		bool parent_selected = false;
		bool selected = false;
		bool parent_enabled = false;

		std::string name = "UnNamed";

		std::vector<Obj*> children;

		struct Comp {

			ComponentType type = ComponentType::UNKNOWN;

			struct {
				float3 pos = float3::zero;
				float3 scale = float3::zero;
				Quat rot = Quat::identity;
				bool is_scale_negative = false;
			} transform;

		}; std::vector<Comp*> comps;

	} object;

private:

	ReturnZ::Obj SetDeleteObject(GameObject* obj, Obj &to_fill);
	void CreateObject(Obj obj);
	
	void SetAction(const ReturnActions& type, void* data);
};