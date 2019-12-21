#include "PanelInspector.h"
#include "ModuleObjects.h"
#include "ModuleRenderer3D.h"
#include "ResourceScript.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentLight.h"
#include "ReturnZ.h"
#include "Alien.h"
#include "ComponentScript.h"

PanelInspector::PanelInspector(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Inspector", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
}

PanelInspector::~PanelInspector()
{
}

void PanelInspector::PanelLogic()
{

	ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	if (ImGui::IsWindowHovered())
		App->camera->is_scene_hovered = false;
	if (App->objects->GetSelectedObjects().size() == 1)
	{
		static bool draw_add = true;
		GameObject* obj = App->objects->GetSelectedObjects().back();
		std::vector<Component*>::iterator item = obj->components.begin();
		for (; item != obj->components.end(); ++item)
		{
			if (*item != nullptr)
			{
				if ((*item)->DrawInspector()) {
					if (!(*item)->not_destroy) {
						to_destroy = (*item);
						delete_panel = &(*item)->not_destroy;
						*delete_panel = !(*delete_panel);
					}
				}
				else {
					draw_add = false;
					break;
				}
			}
		}
		if (draw_add) {
			ButtonAddComponent();
		}
		else {
			draw_add = true;
		}
	}
	else if (App->objects->GetSelectedObjects().size() > 1) {
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Spacing();
			ImGui::Text("Position  ");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(70);
			ImGui::PushID(1);

			std::list<GameObject*> selected = App->objects->GetSelectedObjects();
			auto item = selected.begin();
			float4x4 trans = float4x4::zero;
			bool some_static = false;

			for (; item != selected.end(); ++item) {
				if (*item != nullptr) {
					if ((*item)->is_static) {
						some_static = true;
					}
					if (trans.Equals(float4x4::zero)) {
						trans = (*item)->GetComponent<ComponentTransform>()->global_transformation;
					}
					else {
						trans = trans * (*item)->GetComponent<ComponentTransform>()->global_transformation;
					}
					
				}
			}
			float3 view_pos, view_scale, view_rot;
			Quat rot;
			trans.Decompose(view_pos, rot, view_scale);
			view_rot = rot.ToEulerXYZ();
			view_rot = RadToDeg(view_rot) / selected.size();
			view_pos /= selected.size();
			view_scale /= selected.size();
			float3 original_pos, original_rot, original_scale;
			original_pos = view_pos;
			original_rot = view_rot;
			original_scale = view_scale;

			bool need_refresh_pos = false;
			bool need_refresh_scale = false;
			bool need_refresh_rot = false;

			if (ImGui::DragFloat("X", &view_pos.x, 0.5F, 0, 0, "%.3f", 1, some_static)) {
				need_refresh_pos = true;
			}

			ImGui::PopID();
			ImGui::SameLine();
			ImGui::SetNextItemWidth(70);
			ImGui::PushID(2);
			if (ImGui::DragFloat("Y", &view_pos.y, 0.5F, 0, 0, "%.3f", 1, some_static)) {
				need_refresh_pos = true;
			}

			ImGui::PopID();
			ImGui::SameLine();
			ImGui::SetNextItemWidth(70);
			ImGui::PushID(3);
			if (ImGui::DragFloat("Z", &view_pos.z, 0.5F, 0, 0, "%.3f", 1, some_static)) {
				need_refresh_pos = true;
			}

			ImGui::PopID();
			ImGui::Spacing();

			ImGui::Text("Rotation  ");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(70);
			ImGui::PushID(4);

			if (ImGui::DragFloat("X", &view_rot.x, 0.5F, 0, 0, "%.3f", 1, some_static)) {
				need_refresh_rot = true;
			}

			ImGui::PopID();
			ImGui::SameLine();
			ImGui::SetNextItemWidth(70);
			ImGui::PushID(5);
			if (ImGui::DragFloat("Y", &view_rot.y, 0.5F, 0, 0, "%.3f", 1, some_static)) {
				need_refresh_rot = true;
			}
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::SetNextItemWidth(70);
			ImGui::PushID(6);
			if (ImGui::DragFloat("Z", &view_rot.z, 0.5F, 0, 0, "%.3f", 1, some_static)) {
				need_refresh_rot = true;
			}
			ImGui::PopID();
			ImGui::Spacing();

			ImGui::Text("Scale     ");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(70);
			ImGui::PushID(7);
			if (ImGui::DragFloat("X", &view_scale.x, 0.5F, 0, 0, "%.3f", 1, some_static)) {
				need_refresh_scale = true;
			}
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::SetNextItemWidth(70);
			ImGui::PushID(8);
			if (ImGui::DragFloat("Y", &view_scale.y, 0.5F, 0, 0, "%.3f", 1, some_static)) {
				need_refresh_scale = true;
			}
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::SetNextItemWidth(70);
			ImGui::PushID(9);
			if (ImGui::DragFloat("Z", &view_scale.z, 0.5F, 0, 0, "%.3f", 1, some_static)) {
				need_refresh_scale = true;
			}
			ImGui::PopID();
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (need_refresh_pos) {
				item = selected.begin();
				for (; item != selected.end(); ++item) {
					if (*item != nullptr) {
						(*item)->GetComponent<ComponentTransform>()->AddPosition(view_pos - original_pos);
					}
				}
			}
			else if (need_refresh_rot) {
				item = selected.begin();
				for (; item != selected.end(); ++item) {
					if (*item != nullptr) {
						(*item)->GetComponent<ComponentTransform>()->AddRotation(view_rot - original_rot);
					}
				}
			}
			else if (need_refresh_scale) {
				item = selected.begin();
				for (; item != selected.end(); ++item) {
					if (*item != nullptr) {
						(*item)->GetComponent<ComponentTransform>()->AddScale(view_scale - original_scale);
					}
				}
			}
		}
	}

	ImGui::End();

	DeleteComponentPopup();
}

void PanelInspector::DeleteComponentPopup()
{
	if (delete_panel != nullptr && *delete_panel) {
		ImGui::OpenPopup("Do you want to delete it?");
		ImGui::SetNextWindowSize({ 200,60 });
		if (ImGui::BeginPopupModal("Do you want to delete it?", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::Spacing();
			ImGui::NewLine();
			ImGui::SameLine(40);
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.5F,0,0,1 });
			if (ImGui::Button("Delete")) {
				*delete_panel = !(*delete_panel);
				delete_panel = nullptr;
				App->objects->need_to_delete_objects = true;
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::DELETE_COMPONENT, to_destroy);
				to_destroy = nullptr;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine(100);
			if (ImGui::Button("Cancel")) {
				delete_panel = nullptr;
			}
			ImGui::EndPopup();
		}
	}
}

void PanelInspector::ButtonAddComponent()
{
	ImGui::Spacing();

	// SCRIPT MUST BE THE LAST ONE 
	if (component == 5) { SDL_assert((uint)ComponentType::UNKNOWN == 6); // component == 5 must be changed for the number of script
		if (ImGui::BeginCombo("##Scriptss", std::get<0>(script_info)))
		{
			bool sel = App->StringCmp("Return To Components", std::get<0>(script_info));
			ImGui::Selectable("Return To Components", sel);
			if (ImGui::IsItemClicked())
			{
				std::get<0>(script_info) = "Return To Components";
				component = 0;
			}

			std::vector<Resource*>::iterator item = App->resources->resources.begin();
			for (; item != App->resources->resources.end(); ++item) {
				if (*item != nullptr && (*item)->GetType() == ResourceType::RESOURCE_SCRIPT) {
					ResourceScript* script = (ResourceScript*)(*item);
					if (!script->data_structures.empty()) {
						for (uint i = 0; i < script->data_structures.size(); ++i) {
							bool is_selected = (std::get<0>(script_info) == script->data_structures[i].first.data());
							if (ImGui::Selectable(script->data_structures[i].first.data(), is_selected))
							{
								std::get<0>(script_info) = script->data_structures[i].first.data();
								std::get<1>(script_info) = script->data_structures[i].second;
								std::get<2>(script_info) = script->GetID();
							}
						}
					}
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();

		if (ImGui::Button("Add Component")) {
			if (!App->StringCmp("Return To Components", std::get<0>(script_info))) {
				GameObject* obj = App->objects->GetSelectedObjects().back();
				bool exists = false;
				std::vector<ComponentScript*> scripts = obj->GetComponents<ComponentScript>();
				for (uint i = 0; i < scripts.size(); ++i) {
					if (App->StringCmp(scripts[i]->data_name.data(), std::get<0>(script_info))) {
						exists = true;
						break;
					}
				}
				if (!exists) {
					ComponentScript* comp_script = new ComponentScript(obj);
					comp_script->resourceID = std::get<2>(script_info);
					comp_script->LoadData(std::get<0>(script_info), std::get<1>(script_info));
					std::get<0>(script_info) = "Return To Components";
					component = 0;
					ReturnZ::AddNewAction(ReturnZ::ReturnActions::ADD_COMPONENT, (void*)comp_script);
					if (Time::IsInGameState() && comp_script->need_alien && comp_script->data_ptr != nullptr) {
						Alien* alien = (Alien*)comp_script;
						if (alien != nullptr) {
							alien->Awake();
							alien->Start();
						}
					}
				}
				else {
					LOG("This script is already attached!");
				}
			}
			else {
				LOG("Select a script");
			}
		}
	}

	else {
		ImGui::Combo("##choose component", &component, "Select Component\0Mesh\0Material\0Light\0Camera\0Script\0"); // SCRIPT MUST BE THE LAST ONE

		ImGui::SameLine();

		if (ImGui::Button("Add Component"))
		{
			Component* comp = nullptr;
			switch (component)
			{

			case 0: {
				LOG("Select a Component!");
				break; }

			case 1: {

				if (!App->objects->GetSelectedObjects().back()->HasComponent(ComponentType::MESH))
				{
					comp = new ComponentMesh(App->objects->GetSelectedObjects().back());
					App->objects->GetSelectedObjects().back()->AddComponent(comp);
				}

				else
					LOG("The selected object already has this component!");

				break; }

			case 2: {

				if ((!App->objects->GetSelectedObjects().back()->HasComponent(ComponentType::MATERIAL)) &&
					App->objects->GetSelectedObjects().back()->HasComponent(ComponentType::MESH))
				{
					comp = new ComponentMaterial(App->objects->GetSelectedObjects().back());
					App->objects->GetSelectedObjects().back()->AddComponent(comp);
				}

				else if (App->objects->GetSelectedObjects().back()->HasComponent(ComponentType::MATERIAL))
				{
					LOG("The selected object already has this component!");
				}

				else
					LOG("The object needs a mesh to have a material!");

				break; }

			case 3: {

				if (!App->objects->GetSelectedObjects().back()->HasComponent(ComponentType::LIGHT))
				{
					comp = new ComponentLight(App->objects->GetSelectedObjects().back());
					App->objects->GetSelectedObjects().back()->AddComponent(comp);
				}

				else
					LOG("The selected object already has this component!");

				break; }

			case 4: {

				if (!App->objects->GetSelectedObjects().back()->HasComponent(ComponentType::CAMERA))
				{
					comp = new ComponentCamera(App->objects->GetSelectedObjects().back());
					App->objects->GetSelectedObjects().back()->AddComponent(comp);
					App->renderer3D->selected_game_camera = (ComponentCamera*)comp;
				}

				else
					LOG("The selected object already has this component!");

				break; }
			}

			if (comp != nullptr) {
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::ADD_COMPONENT, comp);
			}
			component = 0;
		}
	}
}

