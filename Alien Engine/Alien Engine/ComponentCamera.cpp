#include "Component.h"
#include "Globals.h"
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

	frustum.nearPlaneDistance = 1.0F;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = DEGTORAD * vertical_fov;
	AspectRatio(16, 9);
	
	camera_color_background = Color(0.1f, 0.1f, 0.1f, 1.0f);
}

ComponentCamera::~ComponentCamera()
{
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

void ComponentCamera::AspectRatio(int width_ratio, int height_ratio)
{
	frustum.horizontalFov = (2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)width_ratio/(float)height_ratio)));
}

void ComponentCamera::Look(const float3& position_to_look)
{
	float3 direction = position_to_look - frustum.pos;

	float3x3 matrix = float3x3::LookAt(frustum.front, direction.Normalized(), frustum.up, float3::unitY);

	frustum.front = matrix.MulDir(frustum.front).Normalized();
	frustum.up = matrix.MulDir(frustum.up).Normalized();
}

// -----------------------------------------------------------------
float* ComponentCamera::GetProjectionMatrix() const
{
	return (float*)frustum.ProjectionMatrix().Transposed().v;
}


float* ComponentCamera::GetViewMatrix() const
{
	return (float*)static_cast<float4x4>(frustum.ViewMatrix()).Transposed().v;
}