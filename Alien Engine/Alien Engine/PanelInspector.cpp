#include "PanelInspector.h"
#include "ModuleObjects.h"
#include "ModuleRenderer3D.h"
#include "ResourceScript.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentLight.h"
#include "ReturnZ.h"
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
	if (App->objects->GetSelectedObject() != nullptr)
	{
		static bool draw_add = true;
		std::vector<Component*>::iterator item = App->objects->GetSelectedObject()->components.begin();
		for (; item != App->objects->GetSelectedObject()->components.end(); ++item)
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
				GameObject* obj = App->objects->GetSelectedObject();
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
					ReturnZ::AddNewAction(ReturnZ::ReturnActions::ADD_COMPONENT, comp_script);
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

				if (!App->objects->GetSelectedObject()->HasComponent(ComponentType::MESH))
				{
					comp = new ComponentMesh(App->objects->GetSelectedObject());
					App->objects->GetSelectedObject()->AddComponent(comp);
				}

				else
					LOG("The selected object already has this component!");

				break; }

			case 2: {

				if ((!App->objects->GetSelectedObject()->HasComponent(ComponentType::MATERIAL)) &&
					App->objects->GetSelectedObject()->HasComponent(ComponentType::MESH))
				{
					comp = new ComponentMaterial(App->objects->GetSelectedObject());
					App->objects->GetSelectedObject()->AddComponent(comp);
				}

				else if (App->objects->GetSelectedObject()->HasComponent(ComponentType::MATERIAL))
				{
					LOG("The selected object already has this component!");
				}

				else
					LOG("The object needs a mesh to have a material!");

				break; }

			case 3: {

				if (!App->objects->GetSelectedObject()->HasComponent(ComponentType::LIGHT))
				{
					comp = new ComponentLight(App->objects->GetSelectedObject());
					App->objects->GetSelectedObject()->AddComponent(comp);
				}

				else
					LOG("The selected object already has this component!");

				break; }

			case 4: {

				if (!App->objects->GetSelectedObject()->HasComponent(ComponentType::CAMERA))
				{
					comp = new ComponentCamera(App->objects->GetSelectedObject());
					App->objects->GetSelectedObject()->AddComponent(comp);
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

