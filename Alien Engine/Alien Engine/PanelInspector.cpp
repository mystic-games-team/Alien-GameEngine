#include "PanelInspector.h"
#include "ModuleObjects.h"
#include "ModuleRenderer3D.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentLight.h"
#include "ReturnZ.h"

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
		if (draw_add)
			ButtonAddComponent();
		else
			draw_add = true;
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

	ImGui::Combo("##choose component", &component, "Select Component\0Mesh\0Material\0Light\0Camera\0");

	ImGui::SameLine();

	if (ImGui::Button("Add Component"))
	{
		Component* comp = nullptr;
		switch (component)
		{

		case 0:
			LOG("Select a Component!");
			break;

		case 1:

			if (!App->objects->GetSelectedObject()->HasComponent(ComponentType::MESH))
			{
				comp = new ComponentMesh(App->objects->GetSelectedObject());
				App->objects->GetSelectedObject()->AddComponent(comp);
			}

			else
				LOG("The selected object already has this component!");

			break;

		case 2:

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

			break;

		case 3:

			if (!App->objects->GetSelectedObject()->HasComponent(ComponentType::LIGHT))
			{
				comp = new ComponentLight(App->objects->GetSelectedObject());
				App->objects->GetSelectedObject()->AddComponent(comp);
			}

			else
				LOG("The selected object already has this component!");

			break;

		case 4:

			if (!App->objects->GetSelectedObject()->HasComponent(ComponentType::CAMERA))
			{
				comp = new ComponentCamera(App->objects->GetSelectedObject());
				App->objects->GetSelectedObject()->AddComponent(comp);
				App->renderer3D->selected_game_camera = (ComponentCamera*)comp;
			}

			else
				LOG("The selected object already has this component!");

			break;
		}

		if (comp != nullptr) {
			ReturnZ::AddNewAction(ReturnZ::ReturnActions::ADD_COMPONENT, comp);
		}
		component = 0;
	}
}
