#pragma once

#include "Component.h"
#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"

class ComponentCamera : public Component {
public:

	ComponentCamera(GameObject* attach);
	virtual ~ComponentCamera();

	void DrawInspector();

	void Reset();
	void SetComponent(Component* component);

	void AspectRatio(int width_ratio, int height_ratio, bool fov_type = 0);

	void Look(const float3& position_to_look);

	float* GetProjectionMatrix() const;

	float* GetViewMatrix() const;

	void DrawFrustum();

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

public:
	Frustum frustum;

	float vertical_fov = 60.0f;
	float horizontal_fov = 91.0f;

	Color camera_color_background{ 0.05f, 0.05f, 0.05f, 1.0f };

	bool projection_changed = false;

	float far_plane = 200.f;

	float near_plane = 0.1f;

	int is_fov_horizontal = 0;
private:

	float4x4 ViewMatrix = float4x4::identity;
	float4x4 ViewMatrixInverse = float4x4::identity;
};
