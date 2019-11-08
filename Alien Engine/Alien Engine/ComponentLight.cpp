#include "ComponentLight.h"
#include "glew/include/glew.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "ComponentTransform.h"
#include "Application.h"

ComponentLight::ComponentLight(GameObject* attach) : Component(attach)
{
	type = ComponentType::LIGHT;
}

ComponentLight::~ComponentLight()
{
	glDisable(light_id);
}

void ComponentLight::LightLogic()
{
	ComponentTransform* transform=(ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
	float pos[] = { transform->GetGlobalPosition().x, transform->GetGlobalPosition().y, transform->GetGlobalPosition().z, 1.F };
	light_id = GL_LIGHT0;
	glEnable(light_id);
	glLightfv(light_id, GL_POSITION, pos);

	// Init
	glLightfv(light_id, GL_AMBIENT, &ambient);
	glLightfv(light_id, GL_DIFFUSE, &diffuse);
}

void ComponentLight::DrawInspector()
{
	ImGui::PushID(this);
	if (ImGui::Checkbox("##CmpActive", &enabled)) {
		if (!enabled)
			OnDisable();
		else
			OnEnable();
	}
	ImGui::PopID();
	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Light", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		RightClickMenu("Light");

		ImGui::Spacing();

		ImGui::ColorEdit3("Ambient Light", &ambient, ImGuiColorEditFlags_Float);
		ImGui::ColorEdit3("Diffuse Light", &diffuse, ImGuiColorEditFlags_Float);

		ImGui::Spacing();
		ImGui::Separator();
	}
	else
		RightClickMenu("Light");
}

void ComponentLight::OnDisable()
{
	glDisable(light_id);
}

void ComponentLight::Reset()
{
	ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
}

void ComponentLight::SetComponent(Component* component)
{
	if (component->GetType() == type) {

		ComponentLight* light = (ComponentLight*)component;

		light_id = light->light_id;
		diffuse = light->diffuse;
		ambient = light->ambient;
	}
}

void ComponentLight::SaveComponent(JSONArraypack* to_save)
{
	to_save->SetNumber("Type", (int)type);
	to_save->SetColor("DiffuseColor", diffuse);
	to_save->SetColor("AmbienColor", ambient);
	to_save->SetBoolean("Enabled", enabled);
}

void ComponentLight::LoadComponent(JSONArraypack* to_load)
{
}
