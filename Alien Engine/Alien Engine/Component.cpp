#include "Component.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "SDL/include/SDL_assert.h"
#include "ComponentLight.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ReturnZ.h"

Component::Component(GameObject* attach)
{
	ID = App->resources->GetRandomID();
	game_object_attached = attach;
}

Component::~Component()
{
}

bool Component::IsEnabled()
{
	return enabled;
}

void Component::SetEnable(bool enable)
{
	enabled = enable;
}

const ComponentType& Component::GetType() const
{
	return type;
}

void Component::RightClickMenu(const char* collapsing_header_name)
{
	if (ImGui::BeginPopupContextItem(collapsing_header_name)) {

		if (ImGui::MenuItem("Reset")) {
			ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			Reset();
		}

		ImGui::Separator();
		
		if (ImGui::MenuItem("Copy Component")) {
			SDL_assert((uint)ComponentType::UNKNOWN == 4); // add new case here
			if (App->objects->component_in_copy != nullptr) {
				delete App->objects->component_in_copy;
				App->objects->component_in_copy = nullptr;
			}
			switch (type) { // add to switch new type
			case ComponentType::LIGHT:
				App->objects->component_in_copy = new ComponentLight(nullptr);
				break;
			case ComponentType::TRANSFORM:
				App->objects->component_in_copy = new ComponentTransform(nullptr);
				break;
			case ComponentType::MESH:
				App->objects->component_in_copy = new ComponentMesh(nullptr);
				break;
			case ComponentType::MATERIAL:
				App->objects->component_in_copy = new ComponentMaterial(nullptr);
				break;
			case ComponentType::CAMERA:
				App->objects->component_in_copy = new ComponentCamera(nullptr);
				break;
			}
			App->objects->component_in_copy->SetComponent(this);
		}

		bool can_paste = false;

		if (App->objects->component_in_copy != nullptr && App->objects->component_in_copy->GetType() == type && App->objects->component_in_copy != this)
			can_paste = true;

		if (ImGui::MenuItem("Paste Component", nullptr, nullptr, can_paste)) {
			ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			SetComponent(App->objects->component_in_copy);
		}

		if (type != ComponentType::TRANSFORM) {
			ImGui::Separator();

			if (ImGui::MenuItem("Enable Component", nullptr, nullptr, !enabled)) {
				enabled = true;
				OnEnable();
			}

			if (ImGui::MenuItem("Disable Component", nullptr, nullptr, enabled)) {
				enabled = false;
				OnDisable();
			}

			if (ImGui::MenuItem("Delete Component", nullptr, nullptr)) {
				not_destroy = false;
			}

			ImGui::Separator();

			bool is_on_top = false;
			bool is_on_bottom = false;

			if (game_object_attached->components.at(1) == this)
				is_on_top = true;
			if (game_object_attached->components.back() == this)
				is_on_bottom = true;

			if (ImGui::MenuItem("Move One Up", nullptr, nullptr, !is_on_top)) {
				App->objects->MoveComponentUp(game_object_attached, this, false);
			}

			if (ImGui::MenuItem("Move On Top", nullptr, nullptr, !is_on_top)) {
				App->objects->MoveComponentUp(game_object_attached, this, true);
			}

			if (ImGui::MenuItem("Move One Down", nullptr, nullptr, !is_on_bottom)) {
				App->objects->MoveComponentDown(game_object_attached, this, false);
			}

			if (ImGui::MenuItem("Move On Bottom", nullptr, nullptr, !is_on_bottom)) {
				App->objects->MoveComponentDown(game_object_attached, this, true);
			}
		}
		
		ImGui::EndPopup();
	}
}
