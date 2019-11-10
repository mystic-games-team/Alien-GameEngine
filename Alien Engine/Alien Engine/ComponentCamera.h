#pragma once

#include "Component.h"
#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"

class ComponentCamera : public Component {
public:

	ComponentCamera(GameObject* attach);
	virtual ~ComponentCamera();

	//void DrawInspector();

	void Reset();
	void SetComponent(Component* component);

	void AspectRatio(int width_ratio, int height_ratio);

	void Look(const float3& position_to_look);

	float* GetProjectionMatrix() const;

	float* GetViewMatrix() const;

	void DrawFrustum();


public:
	Frustum frustum;

	float vertical_fov = 60.0f;

private:

	bool projection_changed = false;

	float4x4 ViewMatrix, ViewMatrixInverse;

	Color camera_color_background{ 0.0f, 0.0f, 0.0f, 1.0f };
};
