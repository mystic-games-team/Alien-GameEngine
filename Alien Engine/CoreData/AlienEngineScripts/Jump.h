#ifndef _JUMP_H_
#define _JUMP_H_

#include "Macros/AlienScripts.h"
#include "..\..\Alien Engine\Alien.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_CLASS_API Jump : public Alien {

public:

	Jump();
	~Jump();

	void Start();

};
ALIEN_INIT_CLASS Jump* CreateJump() { 
	Jump* jump = new Jump();
	return jump;
}

ALIEN_END_HEADER

#endif