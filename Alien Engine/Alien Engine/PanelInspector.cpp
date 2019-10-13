#include "PanelInspector.h"
#include "ModuleObjects.h"
#include "ComponentTransform.h"

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
	ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse);

	if (App->objects->GetSelectedObject() != nullptr) {
		std::vector<Component*>::iterator item = App->objects->GetSelectedObject()->components.begin();
		for (; item != App->objects->GetSelectedObject()->components.end(); ++item) {
			if (*item != nullptr) {
				(*item)->DrawInspector();
			}
		}
	}

	ImGui::End();
}
