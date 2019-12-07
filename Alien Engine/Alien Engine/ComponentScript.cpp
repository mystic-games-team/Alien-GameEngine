#include "ComponentScript.h"
#include "ModuleObjects.h"
#include "Application.h"
#include "ReturnZ.h"
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
}

void ComponentScript::LoadComponent(JSONArraypack* to_load)
{
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
		game_object_attached->components.push_back(this);
		if (need_alien) {
			Alien* alien = (Alien*)data_ptr;
			App->objects->current_scripts.push_back(alien);
			alien->game_object = game_object_attached;
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
