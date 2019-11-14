#include "ComponentLight.h"
#include "glew/include/glew.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ReturnZ.h"

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
	static bool en;
	ImGui::PushID(this);
	en = enabled;
	if (ImGui::Checkbox("##CmpActive", &en)) {
		ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
		enabled = en;
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
		static bool cntl_Z = true;
		ImGui::Spacing();
		static Color col;
		col = ambient;
		if (ImGui::ColorEdit3("Ambient Light", &col, ImGuiColorEditFlags_Float)) {
			if (cntl_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			cntl_Z = false;
			ambient = col;
		}
		else if (!cntl_Z && ImGui::IsMouseReleased(0)) {
			cntl_Z = true;
		}
		col = diffuse;
		if (ImGui::ColorEdit3("Diffuse Light", &col, ImGuiColorEditFlags_Float)) {
			if (cntl_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			cntl_Z = false;
			diffuse = col;
		}
		else if (!cntl_Z && ImGui::IsMouseReleased(0)) {
			cntl_Z = true;
		}
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
	diffuse = to_load->GetColor("DiffuseColor");
	ambient = to_load->GetColor("AmbienColor");
	enabled = to_load->GetBoolean("Enabled");
}
