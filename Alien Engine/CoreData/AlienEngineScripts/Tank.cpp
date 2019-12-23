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
	ControlCamera();
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

	float3 wheels = wheels_transform->GetLocalRotation().ToEulerXYZ();

	if (wheels.x == 0 && wheels.y > 0)
	{
		rotation.y = (-(mouse.x * 180 / width) + 90) + (wheels.y * Maths::Rad2Deg());

		if (mouse.y > (height / 2))
		{
			rotation.x = -180;
			rotation.z = -180;
			rotation.y -= wheels.y * 2 * Maths::Rad2Deg();
		}
		else
		{
			rotation.x = 0;
			rotation.z = 0;
		}
	}

	else if (wheels.x == 0 && wheels.y < 0)
	{
		rotation.y = ((mouse.x * 180 / width)) - (wheels.y * Maths::Rad2Deg());

		if (mouse.y > (height / 2))
		{
			rotation.y += 0;
			rotation.x = 0;
			rotation.z = 0;
		}
		else
		{
			rotation.y -= 270;
			rotation.x = -180;
			rotation.z = -180;
		}
	}

	if (wheels.x == -180 && wheels.y > 0)
	{
		rotation.y = (-(mouse.x * 180 / width)) - (wheels.y * Maths::Rad2Deg());

		if (mouse.y > (height / 2))
		{
			rotation.y += 0;
			rotation.x = 0;
			rotation.z = 0;
		}
		else
		{
			rotation.y -= 270;
			rotation.x = -180;
			rotation.z = -180;
		}
	}

	else if (wheels.x == -180 && wheels.y < 0)
	{
		rotation.y = ((mouse.x * 180 / width)) - (wheels.y * Maths::Rad2Deg());

		if (mouse.y > (height / 2))
		{
			rotation.y += 0;
			rotation.x = 0;
			rotation.z = 0;
		}
		else
		{
			rotation.y -= 270;
			rotation.x = -180;
			rotation.z = -180;
		}
	}

	turret_transform->SetLocalRotation(Quat::FromEulerXYZ(rotation.x * Maths::Deg2Rad(), rotation.y * Maths::Deg2Rad(), rotation.z * Maths::Deg2Rad()));
}

void Tank::ControlCamera()
{
	if (Input::GetKeyDown(SDL_SCANCODE_1)) {
		Camera::SetCurrentCamera((ComponentCamera*)GameObject::FindWithName("FrontCamera")->GetComponent(ComponentType::CAMERA));
	}
	else if (Input::GetKeyDown(SDL_SCANCODE_2)) {
		Camera::SetCurrentCamera((ComponentCamera*)GameObject::FindWithName("RightCamera")->GetComponent(ComponentType::CAMERA));
	}
	else if (Input::GetKeyDown(SDL_SCANCODE_3)) {
		Camera::SetCurrentCamera((ComponentCamera*)GameObject::FindWithName("LeftCamera")->GetComponent(ComponentType::CAMERA));
	}
	else if (Input::GetKeyDown(SDL_SCANCODE_4)) {
		Camera::SetCurrentCamera((ComponentCamera*)GameObject::FindWithName("BackCamera")->GetComponent(ComponentType::CAMERA));
	}
	else if (Input::GetKeyDown(SDL_SCANCODE_5)) {
		Camera::SetCurrentCamera((ComponentCamera*)GameObject::FindWithName("UpCamera")->GetComponent(ComponentType::CAMERA));
	}
	else if (Input::GetMouseButtonDown(Input::MOUSE_BUTTONS::MOUSE_RIGHT_BUTTON)) {
		Camera::SetCurrentCamera((ComponentCamera*)GameObject::FindWithName("MainCamera")->GetComponent(ComponentType::CAMERA));
	}
}

void Tank::OnDrawGizmos()
{
	Gizmos::DrawLine(ray.a, ray.b, Color::Red());
}
