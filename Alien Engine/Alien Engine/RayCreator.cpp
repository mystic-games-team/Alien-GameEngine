#include "RayCreator.h"
#include "MathGeoLib/include/Math/float2.h"
#include "ComponentCamera.h"
#include "Globals.h"

LineSegment& RayCreator::CreateRayScreenToWorld(const float& x, const float& y, const ComponentCamera* camera)
{
	if (camera == nullptr) {
		LOG_ENGINE("Camera passaed in CreateRayScreenToWorld was nullptr!");
		return LineSegment();
	}
	float2 origin = { x,y };

	origin.x = (origin.x - 0.5F) * 2;
	origin.y = -(origin.y - 0.5F) * 2;

	return camera->frustum.UnProjectLineSegment(origin.x, origin.y);
}
