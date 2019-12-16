#include "ComponentScript.h"
#include "ModuleObjects.h"
#include "Application.h"
#include "ReturnZ.h"
#include "ComponentTransform.h"
#include "ResourceScript.h"
#include "Alien.h"
#include "FileNode.h"
#include "ResourcePrefab.h"
#include "Prefab.h"

ComponentScript::ComponentScript(GameObject* attach) : Component(attach)
{
	type = ComponentType::SCRIPT;
}

ComponentScript::~ComponentScript()
{
	if (data_ptr != nullptr) {
		if (need_alien) {
			Alien* alien = (Alien*)data_ptr;
			std::vector<Alien*>::iterator item = App->objects->current_scripts.begin();
			for (; item != App->objects->current_scripts.end(); ++item) {
				if (*item != nullptr && *item == alien) {
					App->objects->current_scripts.erase(item);
					break;
				}
			}
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
					switch (inspector_variables[i].show_as) {
					case InspectorScriptData::ShowMode::INPUT_INT: {
						ImGui::PushID(inspector_variables[i].ptr);
						int* ptr = (int*)inspector_variables[i].ptr;
						ImGui::InputInt(inspector_variables[i].variable_name.data(), ptr);
						ImGui::PopID();
						break; }
					case InspectorScriptData::ShowMode::DRAGABLE_INT: {
						ImGui::PushID(inspector_variables[i].ptr);
						int* ptr = (int*)inspector_variables[i].ptr;
						ImGui::DragInt(inspector_variables[i].variable_name.data(), ptr);
						ImGui::PopID();
						break; }
					case InspectorScriptData::ShowMode::SLIDER_INT: {
						ImGui::PushID(inspector_variables[i].ptr);
						int* ptr = (int*)inspector_variables[i].ptr;
						ImGui::SliderInt(inspector_variables[i].variable_name.data(), ptr, inspector_variables[i].min_slider, inspector_variables[i].max_slider);
						ImGui::PopID();
						break; }
					default:
						break;
					}
					break; }
				case InspectorScriptData::DataType::FLOAT: {
					switch (inspector_variables[i].show_as) {
					case InspectorScriptData::ShowMode::INPUT_FLOAT: {
						ImGui::PushID(inspector_variables[i].ptr);
						float* ptr = (float*)inspector_variables[i].ptr;
						ImGui::InputFloat(inspector_variables[i].variable_name.data(), ptr, 0.5F, 100);
						ImGui::PopID();
						break; }
					case InspectorScriptData::ShowMode::DRAGABLE_FLOAT: {
						ImGui::PushID(inspector_variables[i].ptr);
						float* ptr = (float*)inspector_variables[i].ptr;
						ImGui::DragFloat(inspector_variables[i].variable_name.data(), ptr, 0.5F);
						ImGui::PopID();
						break; }
					case InspectorScriptData::ShowMode::SLIDER_FLOAT: {
						ImGui::PushID(inspector_variables[i].ptr);
						float* ptr = (float*)inspector_variables[i].ptr;
						ImGui::SliderFloat(inspector_variables[i].variable_name.data(), ptr, inspector_variables[i].min_slider, inspector_variables[i].max_slider);
						ImGui::PopID();
						break; }
					default:
						break;
					}
					break; }
				case InspectorScriptData::DataType::BOOL: {
					switch (inspector_variables[i].show_as) {
					case InspectorScriptData::ShowMode::CHECKBOX: {
						ImGui::PushID(inspector_variables[i].ptr);
						bool* ptr = (bool*)inspector_variables[i].ptr;
						ImGui::Checkbox(inspector_variables[i].variable_name.data(), ptr);
						ImGui::PopID();
						break; }
					default:
						break;
					}
					break; }
				case InspectorScriptData::DataType::PREFAB: {
					ImGui::PushID(inspector_variables[i].ptr);
					Prefab* ptr = (Prefab*)inspector_variables[i].ptr;
					ImGui::InputText(inspector_variables[i].variable_name.data(), (ptr->prefab_name.empty()) ? "Prefab: NULL" : ptr->prefab_name.data(), NULL, ImGuiInputTextFlags_ReadOnly);
					if (ImGui::BeginDragDropTarget()) {
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DROP_ID_PROJECT_NODE, ImGuiDragDropFlags_SourceNoDisableHover);
						if (payload != nullptr && payload->IsDataType(DROP_ID_PROJECT_NODE)) {
							FileNode* node = *(FileNode**)payload->Data;
							if (node != nullptr && node->type == FileDropType::PREFAB) {
								std::string path = App->file_system->GetPathWithoutExtension(node->path + node->name);
								path += ".alienPrefab";
								u64 ID = App->resources->GetIDFromAlienPath(path.data());
								if (ID != 0) {
									ResourcePrefab* prefab_ = (ResourcePrefab*)App->resources->GetResourceWithID(ID);
									if (prefab_ != nullptr) {
										ptr->prefabID = ID;
										ptr->prefab_name = std::string(prefab_->GetName());
									}
								}
							}
						}
						ImGui::EndDragDropTarget();
					}

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
			case InspectorScriptData::DataType::FLOAT: {
				float value = *(float*)inspector_variables[i].ptr;
				inspector->SetNumber("float", value);
				break; }
			case InspectorScriptData::DataType::BOOL: {
				bool value = *(bool*)inspector_variables[i].ptr;
				inspector->SetNumber("bool", value);
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
						case InspectorScriptData::DataType::FLOAT: {
							float* value = (float*)inspector_variables[i].ptr;
							*value = inspector->GetNumber("float");
							break; }
						case InspectorScriptData::DataType::BOOL: {
							bool* value = (bool*)inspector_variables[i].ptr;
							*value = inspector->GetNumber("bool");
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
		script->inspector_variables.push_back(InspectorScriptData(variable_name, InspectorScriptData::DataType::INT, ptr, InspectorScriptData::INPUT_INT));
	}
}

void ComponentScript::InspectorDragableInt(int* ptr, const char* ptr_name)
{
	std::string name = typeid(*ptr).name();
	if (!App->StringCmp(name.data(), "int"))
		return;

	std::string variable_name = GetVariableName(ptr_name);

	ComponentScript* script = App->objects->actual_script_loading;
	if (script != nullptr) {
		script->inspector_variables.push_back(InspectorScriptData(variable_name, InspectorScriptData::DataType::INT, ptr, InspectorScriptData::DRAGABLE_INT));
	}
}

void ComponentScript::InspectorSliderInt(int* ptr, const char* ptr_name, const int& min_value, const int& max_value)
{
	std::string name = typeid(*ptr).name();
	if (!App->StringCmp(name.data(), "int"))
		return;

	std::string variable_name = GetVariableName(ptr_name);

	ComponentScript* script = App->objects->actual_script_loading;
	if (script != nullptr) {
		InspectorScriptData inspector(variable_name, InspectorScriptData::DataType::INT, ptr, InspectorScriptData::SLIDER_INT);
		inspector.min_slider = min_value;
		inspector.max_slider = max_value;
		script->inspector_variables.push_back(inspector);
	}
}

void ComponentScript::InspectorInputFloat(float* ptr, const char* ptr_name)
{
	std::string name = typeid(*ptr).name();
	if (!App->StringCmp(name.data(), "float"))
		return;

	std::string variable_name = GetVariableName(ptr_name);

	ComponentScript* script = App->objects->actual_script_loading;
	if (script != nullptr) {
		script->inspector_variables.push_back(InspectorScriptData(variable_name, InspectorScriptData::DataType::FLOAT, ptr, InspectorScriptData::INPUT_FLOAT));
	}
}

void ComponentScript::InspectorDragableFloat(float* ptr, const char* ptr_name)
{
	std::string name = typeid(*ptr).name();
	if (!App->StringCmp(name.data(), "float"))
		return;

	std::string variable_name = GetVariableName(ptr_name);

	ComponentScript* script = App->objects->actual_script_loading;
	if (script != nullptr) {
		script->inspector_variables.push_back(InspectorScriptData(variable_name, InspectorScriptData::DataType::FLOAT, ptr, InspectorScriptData::DRAGABLE_FLOAT));
	}
}

void ComponentScript::InspectorSliderFloat(float* ptr, const char* ptr_name, const float& min_value, const float& max_value)
{
	std::string name = typeid(*ptr).name();
	if (!App->StringCmp(name.data(), "float"))
		return;

	std::string variable_name = GetVariableName(ptr_name);

	ComponentScript* script = App->objects->actual_script_loading;
	if (script != nullptr) {
		InspectorScriptData inspector(variable_name, InspectorScriptData::DataType::FLOAT, ptr, InspectorScriptData::SLIDER_FLOAT);
		inspector.min_slider = min_value;
		inspector.max_slider = max_value;
		script->inspector_variables.push_back(inspector);
	}
}

void ComponentScript::InspectorBool(bool* ptr, const char* ptr_name)
{
	std::string name = typeid(*ptr).name();
	if (!App->StringCmp(name.data(), "bool"))
		return;

	std::string variable_name = GetVariableName(ptr_name);

	ComponentScript* script = App->objects->actual_script_loading;
	if (script != nullptr) {
		script->inspector_variables.push_back(InspectorScriptData(variable_name, InspectorScriptData::DataType::BOOL, ptr, InspectorScriptData::CHECKBOX));
	}
}

void ComponentScript::InspectorPrefab(Prefab* ptr, const char* ptr_name)
{
	if (ptr != nullptr) {
		std::string name = typeid(*ptr).name();
		if (!App->StringCmp(name.data(), "class Prefab"))
			return;

		std::string variable_name = GetVariableName(ptr_name);

		ComponentScript* script = App->objects->actual_script_loading;
		if (script != nullptr) {
			script->inspector_variables.push_back(InspectorScriptData(variable_name, InspectorScriptData::DataType::PREFAB, (void*)ptr, InspectorScriptData::NONE));
		}
	}
	else {
		LOG("Prefab variable must not be a pointer!!");
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
