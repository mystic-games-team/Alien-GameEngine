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
	
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Reset()
{
	camera_color_background = { 0.0f, 0.0f, 0.0f, 1.0f };

	CalculateViewMatrix();

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

float4x4 ComponentCamera::GetViewMatrix()
{
	return ViewMatrix;
}

// -----------------------------------------------------------------
void ComponentCamera::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -Position.Dot(X), -Position.Dot(Y), -Position.Dot(Z), 1.0f);
	
	ViewMatrixInverse = ViewMatrix;

	ViewMatrixInverse.Inverse();
}