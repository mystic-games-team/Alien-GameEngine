#pragma once

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

class ALIEN_ENGINE_API Tank : public Alien {

public:

	Tank();

	virtual ~Tank();


	void Start();
	void Update();

public:

	Prefab bullet;
	GameObject* wheels = nullptr;
	ComponentTransform* wheels_transform = nullptr;
	float max_velocity = 8.0f;
	float3 direction = { 0,0,0 };
	float velocity = 0.0f;
	float friction_force = 0.05f;
	float acceleration = 4.0f;
};

ALIEN_FACTORY Tank* CreateTank() {
	Tank* tank = new Tank();
	SHOW_IN_INSPECTOR_AS_DRAGABLE_FLOAT(tank->max_velocity);
	SHOW_IN_INSPECTOR_AS_DRAGABLE_FLOAT(tank->acceleration);
	SHOW_IN_INSPECTOR_AS_DRAGABLE_FLOAT(tank->friction_force);
	SHOW_IN_INSPECTOR_AS_PREFAB(tank->bullet);
	return tank;
}
ALIEN_FACTORY void DestroyTank(Tank* tank) { delete tank; }
