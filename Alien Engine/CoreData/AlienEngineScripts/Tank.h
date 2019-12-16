#pragma once

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

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
};

ALIEN_INIT_DATA Tank* CreateTank() {
	Tank* tank = new Tank();
	SHOW_IN_INSPECTOR_AS_PREFAB(tank->bullet);
	return tank;
}
ALIEN_DESTROY_DATA void DestroyTank(Tank* tank) { delete tank; }

ALIEN_END_HEADER


