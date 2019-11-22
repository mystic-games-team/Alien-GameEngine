#include "ComponentLight.h"
#include "glew/include/glew.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ReturnZ.h"
#include "ComponentMesh.h"

ComponentLight::ComponentLight(GameObject* attach) : Component(attach)
{
	type = ComponentType::LIGHT;

	bulb = new ComponentMesh(game_object_attached);
	bulb->mesh = App->resources->light_mesh;
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

bool ComponentLight::DrawInspector()
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
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PushID("fnjshdfoiwnec`q");
		ImGui::Checkbox("Print Icon", &print_icon);
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::Text("|");
		ImGui::SameLine();
		ImGui::PushID("kfdsifqnpw");
		ImGui::ColorEdit3("Icon Color", &bulb_icon_color, ImGuiColorEditFlags_Float);
		ImGui::PopID();
		ImGui::Spacing();
		ImGui::Separator();
	}
	else
		RightClickMenu("Light");

	return true;
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
	to_save->SetString("ID", std::to_string(ID));
	to_save->SetBoolean("PrintIcon", print_icon);
	to_save->SetColor("IconColor", bulb_icon_color);
}

void ComponentLight::LoadComponent(JSONArraypack* to_load)
{
	diffuse = to_load->GetColor("DiffuseColor");
	ambient = to_load->GetColor("AmbienColor");
	enabled = to_load->GetBoolean("Enabled");
	ID = std::stoull(to_load->GetString("ID"));
	print_icon = to_load->GetBoolean("PrintIcon");
	bulb_icon_color = to_load->GetColor("IconColor");
}

void ComponentLight::DrawIconLight()
{
	if (bulb != nullptr && print_icon)
	{
		ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
		float3 pos = transform->GetLocalPosition();
		float3 scale = transform->GetLocalScale();
		transform->SetLocalScale(0.2f, 0.18f, 0.2f);
		transform->SetLocalPosition(pos.x - 0.133f, pos.y, pos.z);
		glDisable(GL_LIGHTING);
		glColor3f(bulb_icon_color.r, bulb_icon_color.g, bulb_icon_color.b);
		bulb->DrawPolygon();
		glEnable(GL_LIGHTING);
		transform->SetLocalScale(scale.x, scale.y, scale.z);
		transform->SetLocalPosition(pos.x, pos.y, pos.z);
	}
}