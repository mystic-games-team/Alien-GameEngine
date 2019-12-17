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
	wheels_transform = (ComponentTransform*)wheels->GetComponent(ComponentType::TRANSFORM);
}

void Tank::Update()
{
	float3 movement_vector = { 0, 0, 0 };

	/*if (Input::GetKeyRepeat(SDL_SCANCODE_W))
	{
		movement_vector += (float3(4, 0, 0)*Time::GetDT()) ;
	}

	if (Input::GetKeyRepeat(SDL_SCANCODE_S))
	{
		movement_vector += (float3(-4, 0, 0) * Time::GetDT());
	}

	if (Input::GetKeyRepeat(SDL_SCANCODE_A))
	{
		movement_vector += (float3(0, 0, -4) * Time::GetDT());
	}

	if (Input::GetKeyRepeat(SDL_SCANCODE_D))
	{
		movement_vector += (float3(0, 0, 4) * Time::GetDT());
	}*/

	if (Input::GetKeyRepeat(SDL_SCANCODE_W)) {
		movement_vector += transform->left.Mul(4 * Time::GetDT());
	}

	transform->SetLocalPosition(transform->GetGlobalPosition() + movement_vector);
	float3 euler_rotation = wheels_transform->GetLocalRotation().ToEulerXYZ();
	euler_rotation.y = movement_vector.Normalized().AngleBetween({ 1,0,0 });
	wheels_transform->SetLocalRotation(Quat::FromEulerXYZ(euler_rotation.x, euler_rotation.y, euler_rotation.z));
}