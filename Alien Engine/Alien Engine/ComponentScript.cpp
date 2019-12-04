#include "ComponentScript.h"
#include "ModuleObjects.h"
#include "Application.h"
#include "ReturnZ.h"

ComponentScript::ComponentScript(GameObject* attach) : Component(attach)
{
	type = ComponentType::SCRIPT;
}

ComponentScript::~ComponentScript()
{
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

	if (ImGui::CollapsingHeader("Script", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!inspector_variables.empty()) {
			for (uint i = 0; i < inspector_variables.size(); ++i) {
				if (App->StringCmp(inspector_variables[i].first.data(), "int")) {
					ImGui::InputInt("Test",(int*)inspector_variables[i].second);
				}
			}
		}
	}
	return true;
}

void ComponentScript::SaveComponent(JSONArraypack* to_save)
{
}

void ComponentScript::LoadComponent(JSONArraypack* to_load)
{
}

void ComponentScript::InspectorInputInt(int* ptr)
{
	std::string name = typeid(*ptr).name();
	if (!App->StringCmp(name.data(), "int"))
		return;

	ComponentScript* script = App->objects->actual_script_loading;
	if (script != nullptr) {
		script->inspector_variables.push_back({name.data(), ptr});
	}
	else {
		script = new ComponentScript(App->objects->GetRoot(true)->children.back());
		script->game_object_attached->AddComponent(script);
		script->inspector_variables.push_back({ name.data(), ptr });
		App->objects->actual_script_loading = script;
	}
}
