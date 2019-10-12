#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component {
	
public:

	ComponentTransform();
	virtual ~ComponentTransform();

public:

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
	float4x4 complete_transformation = float4x4::identity;
};
