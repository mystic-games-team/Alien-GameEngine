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
	if (ImGui::IsWindowHovered())
		App->camera->is_scene_hovered = false;
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
		if (ImGui::Checkbox("##Active", &node->enabled)) {
			node->SayChildrenParentIsEnabled(node->enabled);
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID(node);
		if (!node->IsEnabled() || !node->IsParentEnabled())
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 0.5f, 0.5f, 1.f });
		if (ImGui::TreeNodeEx(node->GetName(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | node->IsSelected()))
		{
			if (ImGui::IsItemHovered())
			{
				if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
				{
					ImGui::BeginPopupContextWindow();
					if (ImGui::MenuItem("Move Up"))
					{

					}

					if (ImGui::MenuItem("Move Down"))
					{

					}

					if (ImGui::MenuItem("Destroy"))
					{

					}
					ImGui::EndPopup();
				}
			}
			if (!node->IsEnabled() || !node->IsParentEnabled())
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
			if (!node->IsEnabled() || !node->IsParentEnabled())
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
		if (ImGui::Checkbox("##Active", &node->enabled)) {
			node->SayChildrenParentIsEnabled(node->enabled);
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID(node);
		if (!node->IsEnabled() || !node->IsParentEnabled())
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 0.5f, 0.5f, 1.f });
		ImGui::TreeNodeEx(node->GetName(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth | node->IsSelected());
		if (ImGui::IsItemHovered())
		{
			if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
			{
				ImGui::BeginPopupContextWindow();
				if (ImGui::MenuItem("Move Up"))
				{

				}

				if (ImGui::MenuItem("Move Down"))
				{

				}

				if (ImGui::MenuItem("Destroy"))
				{

				}
				ImGui::EndPopup();
			}
		}
		if (!node->IsEnabled() || !node->IsParentEnabled())
			ImGui::PopStyleColor();
		ImGui::PopID();
		if (ImGui::IsItemClicked()) {
			App->objects->SetNewSelectedObject(node);
		}
		
	}
}
//
//void PanelHierarchy::RightClickMenu(GameObject* clicked)
//{
//	if (ImGui::BeginPopupContextWindow())
//	{
//		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
//		{
//			if (ImGui::MenuItem("Move Up"))
//			{
//
//			}
//
//			if (ImGui::MenuItem("Move Down"))
//			{
//
//			}
//
//			if (ImGui::MenuItem("Destroy"))
//			{
//
//			}
//		}
//
//		if (ImGui::MenuItem("Create Empty Child"))
//		{
//
//		}
//
//		ImGui::EndPopup();
//	}
//}
