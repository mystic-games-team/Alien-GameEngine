#include "Component.h"
#include "imgui/imgui.h"

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

		ImGui::EndPopup();
	}
}
