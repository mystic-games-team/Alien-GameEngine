#include "Bullet.h"

Bullet::Bullet() : Alien()
{
}

Bullet::~Bullet()
{
}

void Bullet::Start()
{
	time = Time::GetGameTime();
	float3 quat = transform->GetLocalRotation().ToEulerXYZ() * Maths::Rad2Deg();
	float3 rot = quat.Mul(bullet_direction);
	transform->SetLocalRotation(Quat::FromEulerXYZ(rot.x,rot.y,rot.z));
}

void Bullet::Update()
{
	transform->SetLocalRotation(transform->GetLocalRotation() * (Quat{ 0, 0, 0.104f, 0.9945f }));
	transform->SetLocalPosition(transform->GetLocalPosition() + bullet_direction.Mul(velocity * Time::GetDT()));
	if ((time + life_time) < Time::GetGameTime())
	{
		GameObject::Destroy(game_object);
	}
}