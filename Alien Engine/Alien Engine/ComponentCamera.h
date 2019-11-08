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

	float4x4 GetViewMatrix() const;

	float4x4 GetProjectionMatrix() const;

	float4x4 OpenGLViewMatrix() const;

	float4x4 OpenGLProjectionMatrix() const;

public:

	float3 X, Y, Z, Position, Reference;

	Frustum frustum;

	float vertical_fov = 60.0f;

private:

	bool projection_changed = false;

	float3 newPos{ 0,0,0 };

	float4x4 ViewMatrix, ViewMatrixInverse;

	Color camera_color_background{ 0.0f, 0.0f, 0.0f, 1.0f };
};
