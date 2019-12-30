#pragma once

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

class ALIEN_ENGINE_API Testtt : public Alien {

public:

	Testtt();
	virtual ~Testtt();

};

ALIEN_FACTORY Testtt* CreateTesttt() {
	Testtt* alien = new Testtt();
	// To show in inspector here

	return alien;
} 
