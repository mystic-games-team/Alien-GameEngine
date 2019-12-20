#include "Bullet.h"

Bullet::Bullet() : Alien()
{
	time = Time::GetGameTime();
}

Bullet::~Bullet()
{
}

void Bullet::Start()
{
	transform->forward = bullet_direction;
}

void Bullet::Update()
{
	transform->SetLocalPosition(transform->GetLocalPosition() + bullet_direction.Mul(velocity * Time::GetDT()));
	if ((time + life_time) < Time::GetGameTime())
	{
		GameObject::Destroy(game_object);
	}
}