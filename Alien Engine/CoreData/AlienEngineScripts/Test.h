#pragma once

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

class ALIEN_ENGINE_API Test : public Alien {

public:

	Test();
	virtual ~Test();

};

ALIEN_FACTORY Test* CreateTest() {
	Test* alien = new Test();
	// To show in inspector here

	return alien;
} 
ALIEN_FACTORY void DestroyTest(Test* alien) { delete alien; }

