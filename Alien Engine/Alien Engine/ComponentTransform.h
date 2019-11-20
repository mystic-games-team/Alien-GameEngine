#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component {
	friend class CompZ;
public:

	ComponentTransform(GameObject* attach);
	ComponentTransform(GameObject* attach, const float3& pos, const Quat& rot, const float3& scale);
	virtual ~ComponentTransform();

	void SetLocalPosition(const float& x, const float& y, const float& z);
	const float3& GetLocalPosition() const;
	const float3& GetGlobalPosition() const;

	void SetLocalScale(const float& x, const float& y, const float& z);
	
	const float3& GetLocalScale() const;
	const float3& GetGlobalScale() const;

	void SetLocalRotation(const float& x, const float& y, const float& z, const float& angle);
	const Quat& GetLocalRotation() const;
	const Quat& GetGlobalRotation() const;

	void SetLocalTransform(float4x4 &transform_matrix);

	void SetGlobalTransform(float4x4& transform_matrix);

	void DrawInspector();

	void SetScaleNegative(const bool& negative);
	bool IsScaleNegative();

	void Reset();
	void SetComponent(Component* component);

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);



private:

	void LookScale();
	void RecalculateTransform();

public:

	float4x4 global_transformation = float4x4::identity;
	float4x4 local_transformation = float4x4::identity;
	

	// to know if flip poly or not
	bool is_scale_negative = false;
private:

	// position
	float3 local_position = { 0,0,0 };
	// sacale
	float3 local_scale = { 0,0,0 };
	// rotation
	Quat local_rotation = { 0,0,0,0 };
	
	float3 euler_rotation = { 0,0,0 };

	bool popup_static = false;
};
