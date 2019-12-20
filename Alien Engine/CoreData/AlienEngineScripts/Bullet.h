#pragma once

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

class ALIEN_ENGINE_API Bullet : public Alien {

public:

	Bullet();
	virtual ~Bullet();

	void Start();
	void Update();

public:

	float life_time = 3.0f;
	float time = 0.0f;
	float velocity = 10.0f;
	float3 bullet_direction = { 0,0,0 };
};

ALIEN_FACTORY Bullet* CreateBullet() {
	Bullet* bullet = new Bullet();
	// To show in inspector here
	SHOW_IN_INSPECTOR_AS_DRAGABLE_FLOAT(bullet->life_time);
	SHOW_IN_INSPECTOR_AS_DRAGABLE_FLOAT(bullet->velocity);
	return bullet;
} 
ALIEN_FACTORY void DestroyBullet(Bullet* alien) { delete alien; }
