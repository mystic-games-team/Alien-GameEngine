#pragma once

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

class ALIEN_ENGINE_API dfgh : public Alien {

public:

	dfgh();
	virtual ~dfgh();

};

ALIEN_FACTORY dfgh* Createdfgh() {
	dfgh* alien = new dfgh();
	// To show in inspector here

	return alien;
} 
