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
			std::list<Alien*>::iterator item = App->objects->current_scripts.begin();
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
	if (component->GetType() == type) {
		ComponentScript* script = (ComponentScript*)component;
		if (App->StringCmp(data_name.data(), script->data_name.data())) {
			if (script->inspector_variables.size() == inspector_variables.size()) {
				for (uint i = 0; i < inspector_variables.size(); ++i) {
					if (inspector_variables[i].variable_type == script->inspector_variables[i].variable_type) {
						switch (inspector_variables[i].variable_type)
						{
						case InspectorScriptData::DataType::INT: {
							int* var = (int*)inspector_variables[i].ptr;
							int* copy = (int*)script->inspector_variables[i].ptr;
							*var = *copy;
							break; }
						case InspectorScriptData::DataType::FLOAT: {
							float* var = (float*)inspector_variables[i].ptr;
							float* copy = (float*)script->inspector_variables[i].ptr;
							*var = *copy;
							break; }
						case InspectorScriptData::DataType::BOOL: {
							bool* var = (bool*)inspector_variables[i].ptr;
							bool* copy = (bool*)script->inspector_variables[i].ptr;
							*var = *copy;
							break; }
						case InspectorScriptData::DataType::PREFAB: {
							Prefab* var = (Prefab*)inspector_variables[i].ptr;
							Prefab* copy = (Prefab*)script->inspector_variables[i].ptr;
							(*var).prefabID = (*copy).prefabID;
							(*var).prefab_name = (*copy).prefab_name;
							break; }
						case InspectorScriptData::DataType::GAMEOBJECT: {
							*inspector_variables[i].obj = *script->inspector_variables[i].obj;
							break; }
						default:
							break;
						}
					}
				}
			}
		}
	}
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
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.16f, 0.29F, 0.5, 1 });
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.16f, 0.29F, 0.5, 1 });
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.16f, 0.29F, 0.5, 1 });
					Prefab* ptr = (Prefab*)inspector_variables[i].ptr;
					ImGui::Button((ptr->prefab_name.empty()) ? "Prefab: NULL" : std::string("Prefab: " + std::string(ptr->prefab_name)).data(), { ImGui::GetWindowWidth() * 0.6F , 0});
					ImGui::PopStyleColor(3);
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
										if (ptr->prefabID != 0) {
											ResourcePrefab* last_prefab = (ResourcePrefab*)App->resources->GetResourceWithID(ptr->prefabID);
											if (last_prefab != nullptr) {
												last_prefab->prefab_references.remove(ptr);
											}
										}
										ptr->prefabID = ID;
										ptr->prefab_name = std::string(prefab_->GetName());
										prefab_->prefab_references.push_back(ptr);
									}
								}
							}
						}
						ImGui::EndDragDropTarget();
					}
					ImGui::PopID();
					ImGui::SameLine();
					ImGui::Text(inspector_variables[i].variable_name.data());
					break; }
				case InspectorScriptData::DataType::GAMEOBJECT: {
					ImGui::PushID(inspector_variables[i].obj);
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.16f, 0.29F, 0.5, 1 });
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.16f, 0.29F, 0.5, 1 });
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.16f, 0.29F, 0.5, 1 });
					ImGui::Button((inspector_variables[i].obj != nullptr && *inspector_variables[i].obj != nullptr) ? std::string("GameObject: " + std::string((*inspector_variables[i].obj)->name)).data() : "GameObject: NULL", { ImGui::GetWindowWidth() * 0.6F , 0 });
					ImGui::PopStyleColor(3);
					if (ImGui::BeginDragDropTarget()) {
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DROP_ID_HIERARCHY_NODES, ImGuiDragDropFlags_SourceNoDisableHover);
						if (payload != nullptr && payload->IsDataType(DROP_ID_HIERARCHY_NODES)) {
							GameObject* obj = *(GameObject**)payload->Data;
							if (obj != nullptr) {
								*inspector_variables[i].obj = obj;
							}
						}
						ImGui::EndDragDropTarget();
					}
					ImGui::PopID();
					ImGui::SameLine();
					ImGui::Text(inspector_variables[i].variable_name.data());
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
			case InspectorScriptData::DataType::PREFAB: {
				Prefab* value = (Prefab*)inspector_variables[i].ptr;
				inspector->SetString("prefab", std::to_string(value->prefabID));
				break; }
			case InspectorScriptData::DataType::GAMEOBJECT: {
				if (inspector_variables[i].obj != nullptr && *inspector_variables[i].obj != nullptr) {
					inspector->SetString("gameobject", std::to_string((*inspector_variables[i].obj)->ID));
				}
				else {
					inspector->SetString("gameobject", "0");
				}
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
						case InspectorScriptData::PREFAB: {
							Prefab* value = (Prefab*)inspector_variables[i].ptr;
							value->prefabID = std::stoull(inspector->GetString("prefab"));
							if (value->prefabID != 0) {
								ResourcePrefab* prefab = (ResourcePrefab*)App->resources->GetResourceWithID(value->prefabID);
								if (prefab != nullptr) {
									value->prefab_name = prefab->name;
									prefab->prefab_references.push_back(value);
								}
								else {
									value->prefabID = 0;
								}
							}
							break; }
						case InspectorScriptData::DataType::GAMEOBJECT: {
							u64 id = std::stoull(inspector->GetString("gameobject"));
							if (id != 0) {
								App->objects->AddScriptObject(id, inspector_variables[i].obj);
							}
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

void ComponentScript::Clone(Component* clone)
{
	clone->enabled = enabled;
	clone->not_destroy = not_destroy;
	ComponentScript* script = (ComponentScript*)clone;
	script->LoadData(data_name.data(), need_alien);
	if (inspector_variables.size() == script->inspector_variables.size()) {
		for (uint i = 0; i < inspector_variables.size(); ++i) {
			if (script->inspector_variables[i].variable_type == inspector_variables[i].variable_type) {
				switch (inspector_variables[i].variable_type) {
				case InspectorScriptData::DataType::BOOL: {
					bool variable = *(bool*)inspector_variables[i].ptr;
					bool* script_var = (bool*)script->inspector_variables[i].ptr;
					*script_var = variable;
					break; }
				case InspectorScriptData::DataType::INT: {
					int variable = *(int*)inspector_variables[i].ptr;
					int* script_var = (int*)script->inspector_variables[i].ptr;
					*script_var = variable;
					break; }
				case InspectorScriptData::DataType::FLOAT: {
					float variable = *(float*)inspector_variables[i].ptr;
					float* script_var = (float*)script->inspector_variables[i].ptr;
					*script_var = variable;
					break; }
				case InspectorScriptData::DataType::PREFAB: {
					Prefab variable = *(Prefab*)inspector_variables[i].ptr;
					Prefab* script_var = (Prefab*)script->inspector_variables[i].ptr;
					(*script_var).prefabID = variable.prefabID;
					(*script_var).prefab_name = variable.prefab_name;
					break; }
				case InspectorScriptData::DataType::GAMEOBJECT: {
					GameObject* obj = *inspector_variables[i].obj;
					*script->inspector_variables[i].obj = obj;
					break; }
				}
			}
		}
	}
}

void ComponentScript::OnDisable()
{
	if (need_alien && data_ptr != nullptr) {
		try {
			Alien* alien = (Alien*)data_ptr;
			alien->OnDisable();
		}
		catch (...)
		{
			try {
				LOG_ENGINE("CODE ERROR IN THE ONDISABLE OF THE SCRIPT: %s", data_name.data());
			}
			catch (...) {
				LOG_ENGINE("UNKNOWN ERROR IN SCRIPTS ONDISABLE");
			}
			// TODO: avisar dalguna manera al usuari
		}
	}
}

void ComponentScript::OnEnable()
{
	if (need_alien && data_ptr != nullptr) {
		try {
			Alien* alien = (Alien*)data_ptr;
			alien->OnEnable();
		}
		catch (...)
		{
			try {
				LOG_ENGINE("CODE ERROR IN THE ONENABLE OF THE SCRIPT: %s", data_name.data());
			}
			catch (...) {
				LOG_ENGINE("UNKNOWN ERROR IN SCRIPTS ONENABLE");
			}
			// TODO: avisar dalguna manera al usuari
		}
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
		LOG_ENGINE("Prefab variable must not be a pointer!!");
	}
}

void ComponentScript::InspectorGameObject(GameObject** ptr, const char* ptr_name)
{
	std::string variable_name = GetVariableName(ptr_name);

	ComponentScript* script = App->objects->actual_script_loading;
	if (script != nullptr) {
		InspectorScriptData data(variable_name, InspectorScriptData::DataType::GAMEOBJECT, nullptr, InspectorScriptData::NONE);
		data.obj = ptr;
		script->inspector_variables.push_back(data);
	}
}

void ComponentScript::LoadData(const char* name, bool is_alien)
{
	need_alien = is_alien;
	void* (*Creator)() = (void* (*)())GetProcAddress(App->scripts_dll, std::string("Create" + std::string(name)).data());
	if (Creator != nullptr) {
		data_name = std::string(name);
		App->objects->actual_script_loading = this;
		try {
			data_ptr = Creator();
		}
		catch (...)
		{
			try {
				LOG_ENGINE("CODE ERROR IN THE CONSTRUCTOR OF THE SCRIPT: %s", name);
			}
			catch (...) {
				LOG_ENGINE("UNKNOWN ERROR IN SCRIPTS CONSTRUCTOR");
			}
			// TODO: avisar dalguna manera al usuari
			return;
		}
		game_object_attached->AddComponent(this);
		if (need_alien) {
			Alien* alien = (Alien*)data_ptr;
			App->objects->current_scripts.push_back(alien);
			alien->game_object = game_object_attached;
			alien->transform = game_object_attached->GetComponent<ComponentTransform>();
			alien->enabled = &enabled;
			strcpy(alien->data_name, name);
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
