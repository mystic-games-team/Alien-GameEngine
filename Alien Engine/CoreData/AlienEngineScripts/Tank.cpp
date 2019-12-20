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
	float3 euler_rotation = { 0,0,0 };

	if (Input::GetKeyRepeat(SDL_SCANCODE_W)) 
	{
		velocity += acceleration * Time::GetDT();
	}
	if (Input::GetKeyRepeat(SDL_SCANCODE_S))
	{
		velocity -= acceleration * Time::GetDT();
	}

	velocity = Maths::Clamp(velocity, max_velocity_backward, max_velocity_forward);

	if (Input::GetKeyRepeat(SDL_SCANCODE_A))
	{
		euler_rotation = transform->GetLocalRotation().ToEulerXYZ() * RADTODEG;

		if (!change_angle)
		{
			euler_rotation.y += 15 * Time::GetDT();
		}
		else
		{
			euler_rotation.y -= 15 * Time::GetDT();
		}

		if (euler_rotation.y >= 90 || euler_rotation.y <= -90)
		{
			change_angle = true;
		}
		else
			change_angle = false;

		euler_rotation *= DEGTORAD;
		transform->SetLocalRotation(Quat::FromEulerXYZ(euler_rotation.x, euler_rotation.y, euler_rotation.z));
	}

	if (Input::GetKeyRepeat(SDL_SCANCODE_D))
	{
		euler_rotation = transform->GetLocalRotation().ToEulerXYZ() * RADTODEG;

		if (change_angle)
		{
			euler_rotation.y += 15 * Time::GetDT();
		}
		else
		{
			euler_rotation.y -= 15 * Time::GetDT();
		}

		if (euler_rotation.y >= 90 || euler_rotation.y <= -90)
		{
			change_angle = true;
		}
		else
			change_angle = false;

		euler_rotation *= DEGTORAD;
		transform->SetLocalRotation(Quat::FromEulerXYZ(euler_rotation.x, euler_rotation.y, euler_rotation.z));
	}

	transform->SetLocalPosition(transform->GetGlobalPosition() + transform->forward.Mul(velocity).Mul(Time::GetDT()));

	if (velocity > 0)
	{
		velocity -= friction_force;
		if (velocity < 0)
		{
			velocity = 0;
		}
	}
	else if (velocity < 0)
	{
		velocity += friction_force;
		if (velocity > 0)
		{
			velocity = 0;
		}
	}



	// Shooting
	if (Input::GetMouseButton(Input::MOUSE_LEFT_BUTTON))
	{
		bullet.ConvertToGameObject(transform->GetGlobalPosition());
	}
}