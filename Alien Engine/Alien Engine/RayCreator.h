#pragma once

#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "MathGeoLib/include/Geometry/Ray.h"

class ComponentCamera;

class __declspec(dllexport) RayCreator {
public:

	LineSegment& CreateRayScreenToWorld(const float& x, const float& y, const ComponentCamera* camera);
	//TODO: CreateRay
};