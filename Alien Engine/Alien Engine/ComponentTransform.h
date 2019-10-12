#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component {
	
public:

	ComponentTransform();
	virtual ~ComponentTransform();

	void SetLocalPosition(const int& x, const int& y, const int& z);
	const float3& GetLocalPosition() const;

	void SetLocalScale(const int& x, const int& y, const int& z);
	const float3& GetLocalScale() const;

	void SetLocalRotation(const int& x, const int& y, const int& z, const int& angle);
	const Quat& GetLocalRotation() const;

	void SetGlobalPosition(const int& x, const int& y, const int& z);
	const float3& GetGlobalPosition() const;

	void SetGlobalScale(const int& x, const int& y, const int& z);
	const float3& GetGlobalScale() const;

	void SetGlobalRotation(const int& x, const int& y, const int& z, const int& angle);
	const Quat& GetGlobalRotation() const;


public:

	float4x4 global_transformation = float4x4::identity;
	float4x4 local_transformation = float4x4::identity;

private:

	// position
	float3 local_position = { 0,0,0 };
	float3 global_position = { 0,0,0 };
	// sacale
	float3 local_scale = { 0,0,0 };
	float3 global_scale = { 0,0,0 };
	// rotation
	Quat local_rotation = { 0,0,0,0 };
	Quat global_rotation = { 0,0,0,0 };

	// position, scale & rotation in one matrix
	
};
