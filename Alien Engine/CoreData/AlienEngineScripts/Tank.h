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
};

ALIEN_FACTORY Tank* CreateTank() {
	Tank* tank = new Tank();
	SHOW_IN_INSPECTOR_AS_PREFAB(tank->bullet);
	return tank;
}
ALIEN_FACTORY void DestroyTank(Tank* tank) { delete tank; }
