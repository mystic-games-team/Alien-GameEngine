#include "PanelHierarchy.h"
#include "ModuleObjects.h"
#include "GameObject.h"

PanelHierarchy::PanelHierarchy(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	enabled = true;
	shortcut = App->shortcut_manager->AddShortCut("Hierarchy", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
}


PanelHierarchy::~PanelHierarchy()
{
}

void PanelHierarchy::PanelLogic()
{
	ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse);

	ImGui::Spacing();
	ImGui::Text("GameObjects");
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	if (!App->objects->base_game_object->children.empty()) {
		std::vector<GameObject*>::iterator item = App->objects->base_game_object->children.begin();
		for (; item != App->objects->base_game_object->children.end(); ++item)
		{
			if (*item != nullptr)
			{
				PrintNode(*item);
			}
		}
	}
	ImGui::End();
}

void PanelHierarchy::PrintNode(GameObject* node)
{

	if (!node->children.empty()) 
	{
		ImGui::PushID(node);
		ImGui::Checkbox("##Active", &node->enabled);
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID(node);
		if (!node->IsEnabled())
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.f));
		if (ImGui::TreeNodeEx(node->GetName(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | node->IsSelected()))
		{
			if (!node->IsEnabled())
				ImGui::PopStyleColor();
			if (ImGui::IsItemClicked()) {
				App->objects->SetNewSelectedObject(node);
			}
			std::vector<GameObject*>::iterator item = node->children.begin();
			for (; item != node->children.end(); ++item) 
			{
				if (*item != nullptr) 
				{
					PrintNode(*item);
				}
			}
			ImGui::TreePop();
		}
		else {
			if (!node->IsEnabled())
				ImGui::PopStyleColor();
			if (ImGui::IsItemClicked()) {
				App->objects->SetNewSelectedObject(node);
			}
		}
		ImGui::PopID();
	}
	else 
	{
		ImGui::PushID(node);
		ImGui::Checkbox("##Active", &node->enabled);
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID(node);
		if (!node->IsEnabled())
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.f));
		ImGui::TreeNodeEx(node->GetName(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth | node->IsSelected());
		if (!node->IsEnabled())
			ImGui::PopStyleColor();
		ImGui::PopID();
		if (ImGui::IsItemClicked()) {
			App->objects->SetNewSelectedObject(node);
		}
	}
}
