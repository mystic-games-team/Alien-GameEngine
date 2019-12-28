#include "RayCreator.h"
#include "MathGeoLib/include/Math/float2.h"
#include "ComponentCamera.h"
#include "Globals.h"
#include "Application.h"
#include "PanelGame.h"
LineSegment RayCreator::CreateRayScreenToWorld(const float& x, const float& y, const ComponentCamera* camera)
{
	if (camera == nullptr) {
		LOG_ENGINE("Camera passaed in CreateRayScreenToWorld was nullptr!");
		return LineSegment();
	}
#ifndef GAME_VERSION
	float2 origin = { x / App->ui->panel_game->width,y / App->ui->panel_game->height };
#else
	float2 origin = { x,y };
#endif
	origin.x = (origin.x - 0.5F) * 2;
	origin.y = -(origin.y - 0.5F) * 2;

	return camera->frustum.UnProjectLineSegment(origin.x, origin.y);
}

Ray RayCreator::CreateRay(const float3& origin, const float3& direction)
{
	return Ray(origin, direction);
}
