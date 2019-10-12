#include "ComponentTransform.h"

ComponentTransform::ComponentTransform() : Component()
{
	type = ComponentType::TRANSFORM;
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::SetLocalPosition(const int& x, const int& y, const int& z)
{
	local_position.x = x;
	local_position.y = y;
	local_position.z = z;

	// TODO change the global matrix & global position & children
}

const float3& ComponentTransform::GetLoaclPosition() const
{
	return local_position;
}

void ComponentTransform::SetLocalScale(const int& x, const int& y, const int& z)
{
	local_scale.x = x;
	local_scale.y = y;
	local_scale.z = z;

	// TODO change the global matrix & global scale & children 
}

const float3& ComponentTransform::GetLoaclScale() const
{
	return local_scale;
}

void ComponentTransform::SetLocalRotation(const int& x, const int& y, const int& z, const int& angle)
{
	local_rotation.x = x;
	local_rotation.y = y;
	local_rotation.z = z;
	local_rotation.w = angle;

	// TODO change the global matrix & global rotation & children 
}

const Quat& ComponentTransform::GetLoaclRotation() const
{
	return local_rotation;
}
