#pragma once

#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "MathGeoLib/include/Geometry/Ray.h"

class ComponentCamera;

class __declspec(dllexport) RayCreator {
public:

	LineSegment& CreateRayScreenToWorld(const float& x, const float& y, const ComponentCamera* camera);
	Ray& CreateRay(const float3& origin, const float3& direction);
};