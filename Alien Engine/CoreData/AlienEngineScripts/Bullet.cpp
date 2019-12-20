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

}

void Bullet::Update()
{
	if ((time + life_time) < Time::GetGameTime())
	{
		GameObject::Destroy(game_object);
	}
}