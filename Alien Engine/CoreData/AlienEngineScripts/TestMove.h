#pragma once

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

class ALIEN_ENGINE_API TestMove : public Alien {

public:

	TestMove();
	virtual ~TestMove();
	void Update();

	float velocity = 10;
};

ALIEN_FACTORY TestMove* CreateTestMove() {
	TestMove* alien = new TestMove();
	// To show in inspector here
	SHOW_IN_INSPECTOR_AS_INPUT_FLOAT(alien->velocity);
	return alien;
} 
