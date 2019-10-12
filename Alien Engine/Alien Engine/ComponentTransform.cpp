#include "ComponentTransform.h"
#include "GameObject.h"

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

const float3& ComponentTransform::GetLocalPosition() const
{
	return local_position;
}

void ComponentTransform::SetLocalScale(const int& x, const int& y, const int& z)
{
	float3 difference = float3(x, y, z) - local_scale;

	local_scale.x = x;
	local_scale.y = y;
	local_scale.z = z;

	// TODO change the global matrix & global size & children 
}

const float3& ComponentTransform::GetLocalScale() const
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

const Quat& ComponentTransform::GetLocalRotation() const
{
	return local_rotation;
}

void ComponentTransform::SetGlobalPosition(const int& x, const int& y, const int& z)
{
	global_position.x = x;
	global_position.y = y;
	global_position.z = z;

	// TODO change the global matrix & local pos & children 
}

const float3& ComponentTransform::GetGlobalPosition() const
{
	return global_position;
}

void ComponentTransform::SetGlobalScale(const int& x, const int& y, const int& z)
{
	global_scale.x = x;
	global_scale.y = y;
	global_scale.z = z;

	// TODO change the global matrix & local scale & children 
}

const float3& ComponentTransform::GetGlobalScale() const
{
	return global_scale;
}

void ComponentTransform::SetGlobalRotation(const int& x, const int& y, const int& z, const int& angle)
{
	global_rotation.x = x;
	global_rotation.y = y;
	global_rotation.z = z;
	global_rotation.w = angle;

	// TODO change the global matrix & local angle & children 
}

const Quat& ComponentTransform::GetGlobalRotation() const
{
	return global_rotation;
}

