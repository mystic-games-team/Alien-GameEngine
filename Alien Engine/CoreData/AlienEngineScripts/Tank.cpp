#include "Tank.h"
#include "Bullet.h"
#include "../../Alien Engine/MathGeoLib/include/Geometry/Plane.h"

Tank::Tank() : Alien()
{
}

Tank::~Tank()
{

}

void Tank::Start()
{
	GameObject* wheels = GameObject::FindWithName("Lower_Tank");
	if (wheels != nullptr) 
	{
		wheels_transform = (ComponentTransform*)wheels->GetComponent(ComponentType::TRANSFORM);
	}

	GameObject* turret = GameObject::FindWithName("TankTurret");
	if (turret != nullptr)
	{
		turret_transform = (ComponentTransform*)turret->GetComponent(ComponentType::TRANSFORM);
	}
}

void Tank::Update()
{
	Movement();
	Rotation();
	Shoot();
}

void Tank::Shoot()
{
	// Shooting
	if (Input::GetMouseButtonDown(Input::MOUSE_LEFT_BUTTON))
	{
		GameObject* bullet_created = bullet.ConvertToGameObject({ transform->GetGlobalPosition().x,transform->GetGlobalPosition().y + 1.5f,transform->GetGlobalPosition().z });

		if (bullet_created != nullptr)
		{
			Bullet* minion = (Bullet*)bullet_created->GetComponentScript("Bullet");
			minion->bullet_direction = turret_transform->forward;
			// TODO: Recoil
		}
	}
}

void Tank::Movement()
{
	if (Input::GetKeyRepeat(SDL_SCANCODE_W))
	{
		velocity += acceleration * Time::GetDT();
	}
	if (Input::GetKeyRepeat(SDL_SCANCODE_S))
	{
		velocity -= acceleration * Time::GetDT();
	}

	velocity = Maths::Clamp(velocity, max_velocity_backward, max_velocity_forward);

	transform->SetLocalPosition(transform->GetGlobalPosition() + wheels_transform->forward * velocity * Time::GetDT());

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
}

void Tank::Rotation()
{
	if (Input::GetKeyRepeat(SDL_SCANCODE_A))
	{
		angle += turning_velocity * Time::GetDT();
		wheels_transform->SetLocalRotation(Quat::FromEulerXYZ(0, angle * Maths::Deg2Rad(), 0));
	}

	if (Input::GetKeyRepeat(SDL_SCANCODE_D))
	{
		angle -= turning_velocity * Time::GetDT();
		wheels_transform->SetLocalRotation(Quat::FromEulerXYZ(0, angle * Maths::Deg2Rad(), 0));
	}

	float2 mouse = { Input::GetMousePosition().x, Input::GetMousePosition().y };
	float3 rotation = turret_transform->GetLocalRotation().ToEulerXYZ();
	int width = Screen::GetWidth();
	int middle = width / 2;
	int height = Screen::GetHeight();

	if (mouse.x > middle) { // negative angle
		//int angle_inverted = (mouse.x / width) * 100;
		//rotation.y = -(90 - angle_inverted);
		rotation.y = -(mouse.x / width) * 100;
	}
	else if (mouse.x < middle) { // positive angle
		//int angle_inverted = (mouse.x / middle) * 100;
		//rotation.y = (90 - angle_inverted);
		rotation.y = (mouse.x / middle) * 100;
	}
	else {
		rotation.y = 0;
	}

	turret_transform->SetLocalRotation(Quat::FromEulerXYZ(0, rotation.y * Maths::Deg2Rad(), 0));
}

void Tank::OnDrawGizmos()
{
	Gizmos::DrawLine(ray.a, ray.b, Color::Red());
}
