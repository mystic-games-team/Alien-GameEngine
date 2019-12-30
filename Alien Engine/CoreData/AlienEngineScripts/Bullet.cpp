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
	ComponentTransform* t_tr = (ComponentTransform*)GameObject::FindWithName("TankTurret")->GetComponent(ComponentType::TRANSFORM);
	transform->SetLocalRotation(t_tr->GetLocalRotation());
}

void Bullet::Update()
{
	// Bullet Rotation
	transform->SetLocalRotation(transform->GetLocalRotation() * (Quat{ 0, 0, 0.104f, 0.9945f }));
	
	transform->SetLocalPosition(transform->GetLocalPosition() + bullet_direction.Mul(velocity * Time::GetDT()));
	if ((time + life_time) < Time::GetGameTime())
	{
		GameObject::DestroyInstantly(game_object);
	}
}

void Bullet::CleanUp()
{
	int i = 0;
}
