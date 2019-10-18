#include "PanelInspector.h"
#include "ModuleObjects.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentLight.h"

PanelInspector::PanelInspector(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Inspector", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
	enabled = true;
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
		std::vector<Component*>::iterator item = App->objects->GetSelectedObject()->components.begin();
		for (; item != App->objects->GetSelectedObject()->components.end(); ++item)
		{
			if (*item != nullptr)
			{
				(*item)->DrawInspector();
			}
		}
	}

	ImGui::Spacing();

	ImGui::Combo("##choose component", &component, "Select Component\0Mesh\0Material\0Light\0");

	ImGui::SameLine();

	if (ImGui::Button("Add Component"))
	{
		switch (component)
		{

		case 0:
			LOG("Select a Component!");

		case 1:

			if (!App->objects->GetSelectedObject()->CheckComponent(ComponentType::MESH))
			{
				ComponentMesh* mesh = new ComponentMesh(App->objects->GetSelectedObject());
				App->objects->GetSelectedObject()->AddComponent(mesh);
			}

			else
				LOG("The selected object already has this component!");

			break;

		case 2:

			if ((!App->objects->GetSelectedObject()->CheckComponent(ComponentType::MATERIAL)) &&
				App->objects->GetSelectedObject()->CheckComponent(ComponentType::MESH))
			{
				ComponentMaterial* material = new ComponentMaterial(App->objects->GetSelectedObject());
				App->objects->GetSelectedObject()->AddComponent(material);
			}

			else if (App->objects->GetSelectedObject()->CheckComponent(ComponentType::MATERIAL))
			{
				LOG("The selected object already has this component!");
			}

			else
				LOG("The object needs a mesh to have a material!");

			break;

		case 3:

			if (!App->objects->GetSelectedObject()->CheckComponent(ComponentType::LIGHT))
			{
				ComponentLight* light = new ComponentLight(App->objects->GetSelectedObject());
				App->objects->GetSelectedObject()->AddComponent(light);
			}

			else
				LOG("The selected object already has this component!");

			break;

		}
	}

	ImGui::End();
}
