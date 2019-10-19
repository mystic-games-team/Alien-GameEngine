#include "Component.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "SDL/include/SDL_assert.h"
#include "ComponentLight.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

Component::Component(GameObject* attach)
{
	game_object_attached = attach;
}

Component::~Component()
{
}

bool Component::IsEnabled()
{
	return enabled;
}

const ComponentType& Component::GetType() const
{
	return type;
}

void Component::RightClickMenu(const char* collapsing_header_name)
{
	if (ImGui::BeginPopupContextItem(collapsing_header_name)) {

		if (ImGui::MenuItem("Reset"))
			Reset();

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
			}
			App->objects->component_in_copy->SetComponent(this);
		}

		bool can_paste = false;

		if (App->objects->component_in_copy != nullptr && App->objects->component_in_copy->GetType() == type && App->objects->component_in_copy != this)
			can_paste = true;

		if (ImGui::MenuItem("Paste Component", nullptr, nullptr, can_paste))
			SetComponent(App->objects->component_in_copy);

		ImGui::EndPopup();
	}
}
