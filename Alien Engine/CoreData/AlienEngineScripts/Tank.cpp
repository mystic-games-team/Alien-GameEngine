#include "Tank.h"

Tank::Tank() : Alien()
{
}

Tank::~Tank()
{

}

void Tank::Start()
{
	wheels = GameObject::FindWithName("Lower_Tank");
	if (wheels != nullptr) 
	{
		wheels_transform = (ComponentTransform*)wheels->GetComponent(ComponentType::TRANSFORM);
	}
}

void Tank::Update()
{
	if (wheels == nullptr) 
	{
		return;
	}

	float3 accel_vector = { 0, 0, 0 };

	if (Input::GetKeyRepeat(SDL_SCANCODE_W)) 
	{
		accel_vector += transform->forward.Mul(acceleration * Time::GetDT());
	}
	if (Input::GetKeyRepeat(SDL_SCANCODE_S))
	{
		accel_vector -= transform->forward.Mul(acceleration * Time::GetDT());
	}

	if (Input::GetKeyRepeat(SDL_SCANCODE_A))
	{
		accel_vector -= transform->right.Mul(acceleration * Time::GetDT());
	}

	if (Input::GetKeyRepeat(SDL_SCANCODE_D))
	{
		accel_vector += transform->right.Mul(acceleration * Time::GetDT());
	}

	// Direction should only be incremented if the input is entering

	if (velocity < max_velocity)
	{
		velocity += accel_vector.Length();
		direction += accel_vector.Normalized();
		Debug::Log("D_X: %f", direction.x);
		Debug::Log("D_Y: %f", direction.y);
		Debug::Log("D_Z: %f", direction.z);
		if (velocity > max_velocity)
			velocity = max_velocity;
	}

	transform->SetLocalPosition(transform->GetGlobalPosition() + direction.Mul(velocity).Mul(Time::GetDT()));
	Debug::Log("X: %f", accel_vector.x);
	Debug::Log("Y: %f", accel_vector.y);
	Debug::Log("Z: %f", accel_vector.z);
	
	if (velocity > 0)
	{
		velocity -= friction_force;
		if (velocity < 0)
		{
			velocity = 0;
		}
	}
	else
		direction = { 0,0,0 };

	

	/*float3 euler_rotation = wheels_transform->GetLocalRotation().ToEulerXYZ();
	euler_rotation.y = accel_vector.Normalized().AngleBetween({ 1,0,0 });
	wheels_transform->SetLocalRotation(Quat::FromEulerXYZ(euler_rotation.x, euler_rotation.y, euler_rotation.z));*/
}