#include "Component.h"
#include "ComponentCamera.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "ComponentTransform.h"

ComponentCamera::ComponentCamera(GameObject* attach): Component(attach)
{
	type = ComponentType::CAMERA;

	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 10.0f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = DegToRad(vertical_fov);
	AspectRatio(16, 9);
	
	camera_color_background = Color(0.1f, 0.1f, 0.1f, 1.0f);
	projection_changed = true;
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Reset()
{
	camera_color_background = { 0.0f, 0.0f, 0.0f, 1.0f };

	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 5.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
}

void ComponentCamera::SetComponent(Component* component)
{
	if (component->GetType() == type) {

		ComponentCamera* camera = (ComponentCamera*)component;

		camera_color_background = camera->camera_color_background;
	}
}

void ComponentCamera::AspectRatio(int width_ratio, int height_ratio)
{
	float ratio = width_ratio / height_ratio;

	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ratio);
	projection_changed = true;
}

// -----------------------------------------------------------------
float4x4 ComponentCamera::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

// -----------------------------------------------------------------
float4x4 ComponentCamera::GetProjectionMatrix() const
{
	//return frustum.ProjectionMatrix();
	return float4x4::D3DPerspProjRH(frustum.nearPlaneDistance, frustum.farPlaneDistance, frustum.NearPlaneWidth(), frustum.NearPlaneHeight());
}


float4x4 ComponentCamera::OpenGLViewMatrix() const
{
	float4x4 view_matrix;

	view_matrix = frustum.ViewMatrix();
	view_matrix.Transpose();

	return view_matrix;
}

// -----------------------------------------------------------------
float4x4 ComponentCamera::OpenGLProjectionMatrix() const
{
	float4x4 view_matrix;

	view_matrix = frustum.ProjectionMatrix();
	view_matrix.Transpose();

	return view_matrix;
}
