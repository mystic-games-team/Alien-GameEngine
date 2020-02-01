#pragma once

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

class ALIEN_ENGINE_API TestMove : public Alien {

public:

	TestMove();
	virtual ~TestMove();
	void Update();

	float velocity = 10;
	float X = 0;
	float Y = 0;
};

ALIEN_FACTORY TestMove* CreateTestMove() {
	TestMove* alien = new TestMove();
	// To show in inspector here
	SHOW_IN_INSPECTOR_AS_INPUT_FLOAT(alien->velocity);
	SHOW_IN_INSPECTOR_AS_INPUT_FLOAT(alien->X);
	SHOW_IN_INSPECTOR_AS_INPUT_FLOAT(alien->Y);
	return alien;
} 
