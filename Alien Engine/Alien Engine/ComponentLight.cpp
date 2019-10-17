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
	if (light_active)
	{
		ComponentTransform* transform=(ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
		float pos[] = { transform->GetGlobalPosition().x, transform->GetGlobalPosition().y, transform->GetGlobalPosition().z, 1.0f };
		light_id = GL_LIGHT0;
		glEnable(light_id);
		glLightfv(light_id, GL_POSITION, pos);
		glLightfv(light_id, GL_AMBIENT, &ambient);
		glLightfv(light_id, GL_DIFFUSE, &diffuse);
	}

	else
		glDisable(light_id);

}

void ComponentLight::DrawInspector()
{
	ImGui::PushID(this);

	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Light Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Checkbox("Active Light", &light_active);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::ColorEdit3("Ambient Light", &ambient, ImGuiColorEditFlags_Float);
		ImGui::ColorEdit3("Diffuse Light", &diffuse, ImGuiColorEditFlags_Float);

		ImGui::PopID();
	}
}
