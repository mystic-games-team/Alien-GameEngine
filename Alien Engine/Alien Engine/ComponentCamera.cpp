#include "Component.h"
#include "Globals.h"
#include "ComponentCamera.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "ComponentTransform.h"
#include "ModuleObjects.h"
#include "Gizmos.h"
#include "Application.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Maths.h"
#include "imgui/imgui.h"
#include "ReturnZ.h"
#include "ModuleRenderer3D.h"
#include "ComponentMesh.h"

ComponentCamera::ComponentCamera(GameObject* attach): Component(attach)
{
	type = ComponentType::CAMERA;

	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3::zero();
	frustum.front = float3::unitZ();
	frustum.up = float3::unitY();

	frustum.nearPlaneDistance = near_plane;
	frustum.farPlaneDistance = far_plane;
	frustum.verticalFov = Maths::Deg2Rad() * vertical_fov;
	AspectRatio(16, 9);

	camera_color_background = Color(0.1f, 0.1f, 0.1f, 1.0f);

	if (attach != nullptr)
	{
		if (App->renderer3D->actual_game_camera == nullptr)
		{
			App->renderer3D->actual_game_camera = this;
		}
		App->objects->game_cameras.push_back(this);
	}
	
#ifndef GAME_VERSION
	mesh_camera = new ComponentMesh(game_object_attached);
	mesh_camera->mesh = App->resources->camera_mesh;
#endif
}

ComponentCamera::~ComponentCamera()
{
	std::vector<ComponentCamera*>::iterator item = App->objects->game_cameras.begin();
	for (; item != App->objects->game_cameras.end(); ++item) {
		if (*item != nullptr && *item == this) {
			App->objects->game_cameras.erase(item);
			if (App->renderer3D->actual_game_camera == this)
			{
				if (!App->objects->game_cameras.empty())
				{
					App->renderer3D->actual_game_camera = App->objects->game_cameras.front();
					App->ui->actual_name = App->renderer3D->actual_game_camera->game_object_attached->GetName();
				}
				else
					App->renderer3D->actual_game_camera = nullptr;
			}
			if (App->renderer3D->selected_game_camera == this)
			{
				App->renderer3D->selected_game_camera = nullptr;
			}
			break;
		}
	}
#ifndef GAME_VERSION
	delete mesh_camera;
#endif
}

bool ComponentCamera::DrawInspector()
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

	if (ImGui::CollapsingHeader("Camera", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		RightClickMenu("Camera");
		static bool cntrl_z = true;
		ImGui::Spacing();
		static Color col;
		col = camera_color_background;
		if (ImGui::ColorEdit3("Background Color", &col, ImGuiColorEditFlags_Float)) {
			if (cntrl_z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			cntrl_z = false;
			camera_color_background = col;
		}
		else if (!cntrl_z && ImGui::IsMouseReleased(0)) {
			cntrl_z = true;
		}
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		static float sup;
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.25f);
		sup = near_plane;
		if (ImGui::DragFloat("Near Plane", &sup, 1, 0.1f, far_plane - 0.1f, "%.1f"))
		{
			if (cntrl_z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			cntrl_z = false;
			near_plane = sup;
			frustum.nearPlaneDistance = near_plane;
			App->renderer3D->UpdateCameraMatrix(this);
		}
		else if (!cntrl_z && ImGui::IsMouseReleased(0)) {
			cntrl_z = true;
		}
		ImGui::SameLine();

		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.25f);
		sup = far_plane;
		if (ImGui::DragFloat("Far Plane", &sup, 1, near_plane + 0.1f, 1000, "%.1f"))
		{
			if (cntrl_z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			cntrl_z = false;
			far_plane = sup;
			frustum.farPlaneDistance = far_plane;
			App->renderer3D->UpdateCameraMatrix(this);
		}
		else if (!cntrl_z && ImGui::IsMouseReleased(0)) {
			cntrl_z = true;
		}
		ImGui::Spacing();
		ImGui::Spacing();
		
		if (is_fov_horizontal!=0)
		{
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.4f);
			sup = horizontal_fov;
			if (ImGui::DragFloat("FOV ", &sup, 1, 1, 163, "%.1f"))
			{
				if (cntrl_z)
					ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
				cntrl_z = false;
				horizontal_fov = sup;
				frustum.horizontalFov = horizontal_fov * Maths::Deg2Rad();
				AspectRatio(16, 9, true);
				vertical_fov = frustum.verticalFov * Maths::Rad2Deg();
				App->renderer3D->UpdateCameraMatrix(this);
			}
			else if (!cntrl_z && ImGui::IsMouseReleased(0)) {
				cntrl_z = true;
			}
		}
		else
		{
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.4f);
			sup = vertical_fov;
			if (ImGui::DragFloat("FOV", &sup, 1, 1, 150, "%.1f"))
			{
				if (cntrl_z)
					ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
				cntrl_z = false;
				vertical_fov = sup;
				frustum.verticalFov = vertical_fov * Maths::Deg2Rad();
				AspectRatio(16, 9);
				horizontal_fov = frustum.horizontalFov * Maths::Rad2Deg();
				App->renderer3D->UpdateCameraMatrix(this);
			}
			else if (!cntrl_z && ImGui::IsMouseReleased(0)) {
				cntrl_z = true;
			}
		}

		ImGui::SameLine();


		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.4f);
		ImGui::Combo("## cool fov combp", &is_fov_horizontal, "Vertical\0Horizontal\0");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PushID("vcnsdbiobsdifnidsofnionew");
		ImGui::Checkbox("Print Icon", &print_icon);
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::Text("|");
		ImGui::SameLine();
		ImGui::PushID("fdgdfdgdgserwfew");
		ImGui::ColorEdit4("Icon Color", &camera_icon_color, ImGuiColorEditFlags_Float);
		ImGui::PopID();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}

	else
		RightClickMenu("Camera");

	
	return true;
}

void ComponentCamera::Reset()
{
	camera_color_background = { 0.05f, 0.05f, 0.05f, 1.0f };

	near_plane = 0.1f;
	far_plane = 200.f;
	frustum.nearPlaneDistance = near_plane;
	frustum.farPlaneDistance = far_plane;

	vertical_fov = 60.0f;
	frustum.verticalFov = Maths::Deg2Rad() * vertical_fov;
	AspectRatio(16, 9);
	horizontal_fov = frustum.horizontalFov * Maths::Rad2Deg();
	print_icon = true;
}

void ComponentCamera::SetComponent(Component* component)
{
	if (component->GetType() == type) {

		ComponentCamera* camera = (ComponentCamera*)component;
		
		camera_color_background = camera->camera_color_background;
		near_plane = camera->near_plane;
		far_plane = camera->far_plane;
		frustum = camera->frustum;
		vertical_fov = camera->vertical_fov;
		horizontal_fov = camera->horizontal_fov;
		print_icon = camera->print_icon;
		is_fov_horizontal = camera->is_fov_horizontal;
		camera_icon_color = camera->camera_icon_color;
	}
}

void ComponentCamera::AspectRatio(int width_ratio, int height_ratio, bool fov_type)
{
	if (fov_type == 0)
	{
		frustum.horizontalFov = (2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)width_ratio / (float)height_ratio)));
	}
	else
	{
		frustum.verticalFov = (2.f * atanf(tanf(frustum.horizontalFov * 0.5f) * ((float)height_ratio) / (float)width_ratio));
	}
}

void ComponentCamera::Look(const float3& position_to_look)
{
	float3 direction = position_to_look - frustum.pos;

	float3x3 matrix = float3x3::LookAt(frustum.front, direction.Normalized(), frustum.up, float3::unitY());

	frustum.front = matrix.MulDir(frustum.front).Normalized();
	frustum.up = matrix.MulDir(frustum.up).Normalized();
}

float* ComponentCamera::GetProjectionMatrix() const
{
	return (float*)frustum.ProjectionMatrix().Transposed().v;
}


float* ComponentCamera::GetViewMatrix() const
{
	return (float*)static_cast<float4x4>(frustum.ViewMatrix()).Transposed().v;
}

void ComponentCamera::SetVerticalFov(const float& vertical_fov)
{
	this->vertical_fov = vertical_fov;
	frustum.verticalFov = Maths::Deg2Rad() * vertical_fov;
	AspectRatio(16, 9);
}

float ComponentCamera::GetVerticalFov() const
{
	return vertical_fov;
}

void ComponentCamera::SetHorizontalFov(const float& horizontal_fov)
{
	this->horizontal_fov = horizontal_fov;
	frustum.horizontalFov = Maths::Deg2Rad() * horizontal_fov;
	AspectRatio(16, 9, true);
}

float ComponentCamera::GetHorizontalFov() const
{
	return horizontal_fov;
}

void ComponentCamera::SetFarPlane(const float& far_plane)
{
	this->far_plane = far_plane;
	frustum.farPlaneDistance = far_plane;
}

void ComponentCamera::SetNearPlane(const float& near_plane)
{
	this->near_plane = near_plane;
	frustum.nearPlaneDistance = near_plane;
}

float ComponentCamera::GetFarPlane() const
{
	return far_plane;
}

float ComponentCamera::GetNearPlane() const
{
	return near_plane;
}

void ComponentCamera::SetCameraPosition(const float3& position)
{
	frustum.pos = position;
}

float3 ComponentCamera::GetCameraPosition() const
{
	return frustum.pos;
}

void ComponentCamera::DrawFrustum()
{
	static float3 points[8];
	frustum.GetCornerPoints(points);

	glLineWidth(App->objects->frustum_line_width);
	glColor3f(App->objects->frustum_color.r, App->objects->frustum_color.g, App->objects->frustum_color.b);
	glBegin(GL_LINES);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[1].x, points[1].y, points[1].z);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[4].x, points[4].y, points[4].z);

	glVertex3f(points[4].x, points[4].y, points[4].z);
	glVertex3f(points[5].x, points[5].y, points[5].z);

	glVertex3f(points[0].x, points[0].y, points[0].z);
	glVertex3f(points[2].x, points[2].y, points[2].z);

	glVertex3f(points[2].x, points[2].y, points[2].z);
	glVertex3f(points[3].x, points[3].y, points[3].z);

	glVertex3f(points[1].x, points[1].y, points[1].z);
	glVertex3f(points[3].x, points[3].y, points[3].z);

	glVertex3f(points[1].x, points[1].y, points[1].z);
	glVertex3f(points[5].x, points[5].y, points[5].z);

	glVertex3f(points[4].x, points[4].y, points[4].z);
	glVertex3f(points[6].x, points[6].y, points[6].z);

	glVertex3f(points[2].x, points[2].y, points[2].z);
	glVertex3f(points[6].x, points[6].y, points[6].z);

	glVertex3f(points[6].x, points[6].y, points[6].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glVertex3f(points[5].x, points[5].y, points[5].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);
	
	glVertex3f(points[3].x, points[3].y, points[3].z);
	glVertex3f(points[7].x, points[7].y, points[7].z);

	glEnd();
	glLineWidth(1);
}

void ComponentCamera::DrawIconCamera()
{
	if (mesh_camera != nullptr && print_icon)
	{
		ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
		float3 position = transform->GetGlobalPosition() - frustum.front.Normalized() * 2;
		Quat rotated = transform->GetGlobalRotation() * (Quat{ 0,0,1,0 } * Quat{ 0.7071,0,0.7071,0 });
		float4x4 matrix = float4x4::FromTRS(position, rotated, { 0.1F,0.1F,0.1F });
		glDisable(GL_LIGHTING);
		Gizmos::DrawPoly(mesh_camera->mesh, matrix, camera_icon_color);
		glEnable(GL_LIGHTING);
	}
}

void ComponentCamera::Clone(Component* clone)
{
	clone->enabled = enabled;
	clone->not_destroy = not_destroy;
	ComponentCamera* camera = (ComponentCamera*)clone;
	camera->camera_color_background = camera_color_background;
	camera->camera_icon_color = camera_icon_color;
	camera->enabled = enabled;
	camera->far_plane = far_plane;
	camera->frustum = frustum;
	camera->horizontal_fov = horizontal_fov;
	camera->is_fov_horizontal = is_fov_horizontal;
	camera->near_plane = near_plane;
	camera->print_icon = print_icon;
	camera->projection_changed = projection_changed;
	camera->vertical_fov = vertical_fov;
	camera->ViewMatrix = ViewMatrix;
	camera->ViewMatrixInverse = ViewMatrixInverse;
}

void ComponentCamera::SaveComponent(JSONArraypack* to_save)
{
	to_save->SetNumber("Type", (int)type);
	to_save->SetNumber("VerticalFov", vertical_fov);
	to_save->SetNumber("HoritzontalFov", horizontal_fov);
	to_save->SetColor("BackCol", camera_color_background);
	to_save->SetNumber("FarPlane", far_plane);
	to_save->SetNumber("NearPlane", near_plane);
	to_save->SetNumber("isFovHori", is_fov_horizontal);
	to_save->SetString("ID", std::to_string(ID));
	to_save->SetBoolean("IsGameCamera", (App->renderer3D->actual_game_camera == this) ? true : false);
	to_save->SetBoolean("IsSelectedCamera", (game_object_attached->IsSelected()) ? true : false);
	to_save->SetBoolean("PrintIcon", print_icon);
	to_save->SetColor("IconColor", camera_icon_color);

}

void ComponentCamera::LoadComponent(JSONArraypack* to_load)
{
	vertical_fov = to_load->GetNumber("VerticalFov");
	horizontal_fov = to_load->GetNumber("HoritzontalFov");
	far_plane = to_load->GetNumber("FarPlane");
	near_plane = to_load->GetNumber("NearPlane");
	is_fov_horizontal = to_load->GetNumber("isFovHori");
	camera_color_background = to_load->GetColor("BackCol");
	ID = std::stoull(to_load->GetString("ID"));
	print_icon = to_load->GetBoolean("PrintIcon");
	camera_icon_color = to_load->GetColor("IconColor");
	if (to_load->GetBoolean("IsGameCamera")) {
		App->renderer3D->actual_game_camera = this;
	}
	if (to_load->GetBoolean("IsSelectedCamera")) {
		App->renderer3D->selected_game_camera = this;
	}

	frustum.nearPlaneDistance = near_plane;
	frustum.farPlaneDistance = far_plane;
	frustum.verticalFov = vertical_fov * Maths::Deg2Rad();
	frustum.horizontalFov = horizontal_fov * Maths::Deg2Rad();

	if (game_object_attached != nullptr) {
		ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
		frustum.pos = transform->GetGlobalPosition();
		frustum.front = transform->GetLocalRotation().WorldZ();
		frustum.up = transform->GetLocalRotation().WorldY();
	}
}

