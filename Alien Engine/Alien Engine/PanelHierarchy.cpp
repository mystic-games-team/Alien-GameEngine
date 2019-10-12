#include "PanelHierarchy.h"
#include "ModuleObjects.h"
#include "GameObject.h"

PanelHierarchy::PanelHierarchy(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	enabled = true;
	shortcut = App->shortcut_manager->AddShortCut("Panel Hierarchy", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
}


PanelHierarchy::~PanelHierarchy()
{
}

void PanelHierarchy::PanelLogic()
{
	ImGui::Begin(panel_name.data(), &enabled);

	ImGui::Spacing();
	ImGui::Text("GameObjects");
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	if (!App->objects->base_game_object->children.empty()) {
		PrintNode(App->objects->base_game_object);
	}
		

	ImGui::End();
}

void PanelHierarchy::PrintNode(GameObject* node)
{
	static ImGuiTreeNodeFlags_ flags = ImGuiTreeNodeFlags_None;
	if (node != App->objects->base_game_object) {
		if (!node->children.empty()) {
			ImGui::PushID(node);
			if (ImGui::TreeNodeEx(node->GetName(), ImGuiTreeNodeFlags_SpanAvailWidth)) {
				std::vector<GameObject*>::iterator item = node->children.begin();
				for (; item != node->children.end(); ++item) {
					if (*item != nullptr) {
						PrintNode(*item);
					}
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		else {
			ImGui::PushID(node);
			ImGui::TreeNodeEx(node->GetName(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth);
			ImGui::PopID();
		}
	}
	else {
		std::vector<GameObject*>::iterator item = App->objects->base_game_object->children.begin();
		for (; item != App->objects->base_game_object->children.end(); ++item) {
			if (*item != nullptr) {
				PrintNode(*item);
			}
		}
	}
}
