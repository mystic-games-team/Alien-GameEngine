#include "ComponentLight.h"
#include "glew/include/glew.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "ComponentTransform.h"

ComponentLight::ComponentLight(GameObject* attach) : Component(attach)
{
	type = ComponentType::LIGHT;
}

ComponentLight::~ComponentLight()
{
}

void ComponentLight::LightLogic()
{
	ComponentTransform* transform=(ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
	float pos[] = { transform->GetGlobalPosition().x, transform->GetGlobalPosition().y, transform->GetGlobalPosition().z, 1.0f };
	light_id = GL_LIGHT0;
	glEnable(light_id);
	glLightfv(light_id, GL_POSITION, pos);
	glLightfv(light_id, GL_AMBIENT, &ambient);
	glLightfv(light_id, GL_DIFFUSE, &diffuse);
}

void ComponentLight::DrawInspector()
{
	if (not_destroy)
	{
		ImGui::PushID(this);
		if (ImGui::Checkbox("##CmpActive", &enabled)) {
			if (!enabled)
				glDisable(light_id);
		}
		ImGui::PopID();
		ImGui::SameLine();
	}

	if (ImGui::CollapsingHeader("Light Settings", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Spacing();

		ImGui::ColorEdit3("Ambient Light", &ambient, ImGuiColorEditFlags_Float);
		ImGui::ColorEdit3("Diffuse Light", &diffuse, ImGuiColorEditFlags_Float);

		ImGui::Spacing();
		ImGui::Separator();
	}
}

void ComponentLight::OnDisable()
{
	glDisable(light_id);
}
