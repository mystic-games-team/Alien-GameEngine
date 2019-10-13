#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component {
	
public:

	ComponentTransform(GameObject* attach, const float3& pos, const Quat& rot, const float3& scale);
	virtual ~ComponentTransform();

	void SetLocalPosition(const float& x, const float& y, const float& z);
	const float3& GetLocalPosition() const;

	void SetLocalScale(const float& x, const float& y, const float& z);
	const float3& GetLocalScale() const;

	void SetLocalRotation(const float& x, const float& y, const float& z, const float& angle);
	const Quat& GetLocalRotation() const;

	void DrawInspector();

	void RecalculateTransform();

public:

	float4x4 global_transformation = float4x4::identity;
	float4x4 local_transformation = float4x4::identity;

	bool has_dirty_flag = false;

private:

	// position
	float3 local_position = { 0,0,0 };
	// sacale
	float3 local_scale = { 0,0,0 };
	// rotation
	Quat local_rotation = { 0,0,0,0 };
	
};
