#include "ComponentScript.h"
#include "ModuleObjects.h"
#include "Application.h"
#include "ReturnZ.h"
#include "ComponentTransform.h"
#include "ResourceScript.h"
#include "Alien.h"

ComponentScript::ComponentScript(GameObject* attach) : Component(attach)
{
	type = ComponentType::SCRIPT;
}

ComponentScript::~ComponentScript()
{
	if (data_ptr != nullptr) {
		if (need_alien) {
			Alien* alien = (Alien*)data_ptr;
			if (alien != nullptr) {
				delete alien;
			}
		}
		else {
			void (*Deleter)(void*) = (void (*)(void*))GetProcAddress(App->scripts_dll, std::string("Destroy" + std::string(data_name)).data());
			Deleter(data_ptr);
		}
	}
}

void ComponentScript::Reset()
{
}

void ComponentScript::SetComponent(Component* component)
{
}

bool ComponentScript::DrawInspector()
{
	static bool en;
	ImGui::PushID(this);
	en = enabled;
	if (ImGui::Checkbox("##CmpActive", &en)) {
		// TODO: CNTRL Z SCRIPT
		//ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
		enabled = en;
		if (!enabled)
			OnDisable();
		else
			OnEnable();
	}

	ImGui::PopID();
	ImGui::SameLine();

	if (ImGui::CollapsingHeader(data_name.data(), &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!inspector_variables.empty()) {
			ImGui::Spacing();
			for (uint i = 0; i < inspector_variables.size(); ++i) {
				switch (inspector_variables[i].variable_type)
				{
				case InspectorScriptData::DataType::INT: {
					ImGui::PushID(inspector_variables[i].ptr);
					int* ptr = (int*)inspector_variables[i].ptr;
					ImGui::InputInt(inspector_variables[i].variable_name.data(), ptr);
					ImGui::PopID();
					break; }
				default:
					break;
				}
			}
		}
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}
	return true;
}

void ComponentScript::SaveComponent(JSONArraypack* to_save)
{
	to_save->SetNumber("Type", (int)type);
	to_save->SetString("ID", std::to_string(ID));
	to_save->SetString("ResourceID", std::to_string(resourceID));
	to_save->SetString("DataName", data_name);
	if (inspector_variables.empty()) {
		to_save->SetBoolean("HasInspector", false);
	}
	else {
		to_save->SetBoolean("HasInspector", true);
		JSONArraypack* inspector = to_save->InitNewArray("Inspector");
		for (uint i = 0; i < inspector_variables.size(); ++i) {
			inspector->SetAnotherNode();
			inspector->SetNumber("VarType", inspector_variables[i].variable_type);
			inspector->SetString("Name", inspector_variables[i].variable_name);
			switch (inspector_variables[i].variable_type) {
			case InspectorScriptData::DataType::INT: {
				int value = *(int*)inspector_variables[i].ptr;
				inspector->SetNumber("int", value);
				break; }
			default:
				break;
			}
		}
	}
}

void ComponentScript::LoadComponent(JSONArraypack* to_load)
{
	type = (ComponentType)(int)to_load->GetNumber("Type");
	ID = std::stoull(to_load->GetString("ID"));
	resourceID = std::stoull(to_load->GetString("ResourceID"));
	data_name = to_load->GetString("DataName");
	ResourceScript* script = dynamic_cast<ResourceScript*>(App->resources->GetResourceWithID(resourceID));
	if (resourceID != 0 && script != nullptr) {
		for (uint i = 0; i < script->data_structures.size(); ++i) {
			if (App->StringCmp(data_name.data(), script->data_structures[i].first.data())) {
				LoadData(data_name.data(), script->data_structures[i].second);
				break;
			}
		}
		if (!inspector_variables.empty() && to_load->GetBoolean("HasInspector")) {
			JSONArraypack* inspector = to_load->GetArray("Inspector");
			for (uint i = 0; i < inspector_variables.size(); ++i) {
				for (uint j = 0; j < inspector->GetArraySize(); ++j) {
					std::string val_name = inspector->GetString("Name");
					int val_type = inspector->GetNumber("VarType");
					if (App->StringCmp(inspector_variables[i].variable_name.data(), val_name.data()) && inspector_variables[i].variable_type == val_type) {
						switch (inspector_variables[i].variable_type) {
						case InspectorScriptData::DataType::INT: {
							int* value = (int*)inspector_variables[i].ptr;
							*value = inspector->GetNumber("int");
							break; }
						default:
							break;
						}
					}
					inspector->GetAnotherNode();
				}
				inspector->GetFirstNode();
			}
		}
	}
	else {
		delete this;
	}
}

void ComponentScript::InspectorInputInt(int* ptr, const char* ptr_name)
{
	std::string name = typeid(*ptr).name();
	if (!App->StringCmp(name.data(), "int"))
		return;
	
	std::string variable_name = GetVariableName(ptr_name);

	ComponentScript* script = App->objects->actual_script_loading;
	if (script != nullptr) {
		script->inspector_variables.push_back(InspectorScriptData(variable_name, InspectorScriptData::DataType::INT, ptr));
	}
}

void ComponentScript::LoadData(const char* name, bool is_alien)
{
	need_alien = is_alien;
	void* (*Creator)() = (void* (*)())GetProcAddress(App->scripts_dll, std::string("Create" + std::string(name)).data());
	if (Creator != nullptr) {
		data_name = std::string(name);
		App->objects->actual_script_loading = this;
		data_ptr = Creator();
		game_object_attached->AddComponent(this);
		if (need_alien) {
			Alien* alien = (Alien*)data_ptr;
			App->objects->current_scripts.push_back(alien);
			alien->game_object = game_object_attached;
			alien->transform = game_object_attached->GetComponent<ComponentTransform>();
			alien->enabled = &enabled;
		}
	}
}

std::string ComponentScript::GetVariableName(const char* ptr_name)
{
	std::string ptr_strg(ptr_name);
	std::string variable_name;
	for (uint i = ptr_strg.size() - 1; i >= 0; --i) {
		if (ptr_strg[i] == '&' || ptr_strg[i] == '*' || ptr_strg[i] == '>' || ptr_strg[i] == '.')
			break;
		variable_name = ptr_strg[i] + variable_name;
	}
	return variable_name;
}
