#include "Component.h"
#include "Globals.h"
#include "ComponentCamera.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "ComponentTransform.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "imgui/imgui.h"

ComponentCamera::ComponentCamera(GameObject* attach): Component(attach)
{
	type = ComponentType::CAMERA;

	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = near_plane;
	frustum.farPlaneDistance = far_plane;
	frustum.verticalFov = DEGTORAD * vertical_fov;
	AspectRatio(16, 9);
	
	camera_color_background = Color(0.1f, 0.1f, 0.1f, 1.0f);
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::DrawInspector()
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

	if (ImGui::CollapsingHeader("Camera", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		RightClickMenu("Camera");

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::ColorEdit3("Background Color", &camera_color_background, ImGuiColorEditFlags_Float);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.25f);
		if (ImGui::DragFloat("Near Plane", &near_plane, 1, 0.1f, far_plane - 0.1f, "%.1f"))
		{
			frustum.nearPlaneDistance = near_plane;
		}
		
		ImGui::SameLine();

		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.25f);
		if (ImGui::DragFloat("Far Plane", &far_plane, 1, near_plane + 0.1f, 1000, "%.1f"))
		{
			frustum.farPlaneDistance = far_plane;
		}

		ImGui::Spacing();
		ImGui::Spacing();
		
		if (is_fov_horizontal!=0)
		{
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.4f);
			if (ImGui::DragFloat("FOV ", &horizontal_fov, 1, 1, 163, "%.1f"))
			{
				frustum.horizontalFov = horizontal_fov * DEGTORAD;
				AspectRatio(16, 9, true);
				vertical_fov = frustum.verticalFov * RADTODEG;
			}
		}
		else
		{
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.4f);
			if (ImGui::DragFloat("FOV", &vertical_fov, 1, 1, 150, "%.1f"))
			{
				frustum.verticalFov = vertical_fov * DEGTORAD;
				AspectRatio(16, 9);
				horizontal_fov = frustum.horizontalFov * RADTODEG;
			}
		}

		ImGui::SameLine();


		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.4f);
		ImGui::Combo("## cool fov combp", &is_fov_horizontal, "Vertical\0Horizontal\0");

		ImGui::Spacing();
		ImGui::Separator();
	}

	else
		RightClickMenu("Camera");
}

void ComponentCamera::Reset()
{
	camera_color_background = { 0.0f, 0.0f, 0.0f, 1.0f };
}

void ComponentCamera::SetComponent(Component* component)
{
	if (component->GetType() == type) {

		ComponentCamera* camera = (ComponentCamera*)component;

		camera_color_background = camera->camera_color_background;
	}
}

void ComponentCamera::AspectRatio(int width_ratio, int height_ratio, bool fov_type)
{
	if (!fov_type)
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

	float3x3 matrix = float3x3::LookAt(frustum.front, direction.Normalized(), frustum.up, float3::unitY);

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

void ComponentCamera::DrawFrustum()
{
	static float3 points[8];
	frustum.GetCornerPoints(points);

	glLineWidth(1);
	glColor3f(1, 0, 0);
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
