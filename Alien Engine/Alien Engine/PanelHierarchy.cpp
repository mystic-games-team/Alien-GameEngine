#include "PanelHierarchy.h"
#include "ModuleObjects.h"
#include "GameObject.h"
#include "PanelCreateObject.h"

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
	// active checkbox
	ImGui::PushID(node);
	if (ImGui::Checkbox("##Active", &node->enabled)) {
		node->SayChildrenParentIsEnabled(node->enabled);
	}
	ImGui::PopID();

	ImGui::SameLine();

	ImGui::PushID(node);
	
	bool is_tree_open = ImGui::TreeNodeEx(node->GetName(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | 
		ImGuiTreeNodeFlags_OpenOnDoubleClick | (node->IsSelected() ? ImGuiTreeNodeFlags_Selected : 0) | 
		(node->children.empty() ? ImGuiTreeNodeFlags_Leaf : 0), (!node->IsEnabled() || !node->IsParentEnabled()));

	if (ImGui::IsItemClicked()) {
		App->objects->SetNewSelectedObject(node);
	}
	if (ImGui::IsItemHovered()) {
		object_hovered = node;
	}

	if (ImGui::BeginDragDropTarget()) {
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("node", ImGuiDragDropFlags_SourceNoDisableHover);
		if (payload != nullptr && payload->IsDataType("node")) {
			 // posar if amb el type
			int id = *(int*)payload->Data;
			GameObject* obj = App->objects->GetGameObjectByID(id);
			if (obj != nullptr) {
				App->objects->ReparentGameObject(obj, node);
			}
		}
		ImGui::EndDragDropTarget();
	}
	
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoDisableHover)) {
		ImGui::SetDragDropPayload("node", &node->id, sizeof(int), ImGuiCond_Once);
		ImGui::Text(node->GetName());
		ImGui::EndDragDropSource();
	}

	if (is_tree_open) {
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
	ImGui::PopID();
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

			bool is_on_top = false;
			bool is_on_bottom = false;

			if (object_menu->parent->children.front() == object_menu)
				is_on_top = true;
			if (object_menu->parent->children.back() == object_menu)
				is_on_bottom = true;

			if (ImGui::MenuItem("Move One Up", nullptr, nullptr, !is_on_top))
			{
				App->objects->MoveObjectUp(object_menu, false);
			}

			if (ImGui::MenuItem("Move On Top", nullptr, nullptr, !is_on_top))
			{
				App->objects->MoveObjectUp(object_menu, true);
			}

			if (ImGui::MenuItem("Move One Down", nullptr, nullptr, !is_on_bottom))
			{
				App->objects->MoveObjectDown(object_menu, false);
			}

			if (ImGui::MenuItem("Move On Bottom", nullptr, nullptr, !is_on_bottom))
			{
				App->objects->MoveObjectDown(object_menu, true);
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Create Empty Child"))
			{
				App->objects->CreateEmptyGameObject(object_menu);
			}
			if (ImGui::MenuItem("Remove Object"))
			{
				object_menu->ToDelete();
			}
			ImGui::Separator();
		}
		if (ImGui::MenuItem("Crete Empty GameObject"))
		{
			App->objects->CreateEmptyGameObject(nullptr);
		}
		if (ImGui::MenuItem("Cube"))
		{
			App->objects->CreateBasePrimitive(PrimitiveType::CUBE);
		}
		if (ImGui::MenuItem("Sphere"))
		{
			App->objects->CreateBasePrimitive(PrimitiveType::SPHERE_ALIEN);
		}
		if (ImGui::MenuItem("Rock"))
		{
			App->objects->CreateBasePrimitive(PrimitiveType::ROCK);
		}
		if (ImGui::MenuItem("Torus"))
		{
			App->objects->CreateBasePrimitive(PrimitiveType::TORUS);
		}
		if (ImGui::BeginMenu("Other"))
		{
			if (ImGui::MenuItem("Dodecahedron"))
			{
				App->objects->CreateBasePrimitive(PrimitiveType::DODECAHEDRON);
			}
			if (ImGui::MenuItem("Octahedron"))
			{
				App->objects->CreateBasePrimitive(PrimitiveType::OCTAHEDRON);
			}
			if (ImGui::MenuItem("Icosahedron"))
			{
				App->objects->CreateBasePrimitive(PrimitiveType::ICOSAHEDRON);
			}
			if (ImGui::MenuItem("Klein Bottle"))
			{
				App->objects->CreateBasePrimitive(PrimitiveType::KLEIN_BOTTLE);
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Create..."))
		{
			App->ui->panel_create_object->ChangeEnable();
		}
		ImGui::EndPopup();
	}
	else if (in_menu) {
		in_menu = false;
		object_menu = nullptr;
	}

}