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
		object_hovered = nullptr;
		std::vector<GameObject*>::iterator item = App->objects->base_game_object->children.begin();
		for (; item != App->objects->base_game_object->children.end(); ++item)
		{
			if (*item != nullptr)
			{
				PrintNode(*item);
			}
		}
	}
	RightClickMenu();
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
			if (!node->IsEnabled() || !node->IsParentEnabled())
				ImGui::PopStyleColor();
			if (ImGui::IsItemClicked()) {
				App->objects->SetNewSelectedObject(node);
			}
			if (ImGui::IsItemHovered()) {
				object_hovered = node;
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
			if (ImGui::IsItemHovered()) {
				object_hovered = node;
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
		if (!node->IsEnabled() || !node->IsParentEnabled())
			ImGui::PopStyleColor();
		ImGui::PopID();
		if (ImGui::IsItemClicked()) {
			App->objects->SetNewSelectedObject(node);
		}
		if (ImGui::IsItemHovered()) {
			object_hovered = node;
		}
	}
}
void PanelHierarchy::RightClickMenu()
{
	if (ImGui::BeginPopupContextWindow()) {

		if (!in_menu) {
			in_menu = true;
			object_menu = object_hovered;
			if (object_menu != nullptr) {
				App->objects->SetNewSelectedObject(object_menu);
			}
		}
		if (object_menu != nullptr) {

			if (ImGui::MenuItem("Move One Up"))
			{
				
			}

			if (ImGui::MenuItem("Move On Top"))
			{

			}

			if (ImGui::MenuItem("Move One Down"))
			{

			}

			if (ImGui::MenuItem("Move On Bottom"))
			{

			}

			if (ImGui::MenuItem("Create Empty Child"))
			{
				App->objects->CreateEmptyGameObject(object_menu);
			}
			if (ImGui::MenuItem("Destroy"))
			{
				object_menu->ToDelete();
			}
			ImGui::Separator();
		}
		if (ImGui::MenuItem("Crete Empty GameObject"))
		{
			App->objects->CreateEmptyGameObject(nullptr);
		}
		// We should create cube, sphere bla bla...
		ImGui::EndPopup();
	}
	else if (in_menu) {
		in_menu = false;
		object_menu = nullptr;
	}

}