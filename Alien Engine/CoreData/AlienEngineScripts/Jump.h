#ifndef _JUMP_H_
#define _JUMP_H_

#include "AlienScripts.h"
#include "..\..\Alien Engine\Alien.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_CLASS_API Jump : Alien {

public:

	Jump();
	~Jump();

	void Start();

};
ALIEN_INIT_CLASS Jump* CreateJump(std::vector<std::pair<std::string, void*>>* ins) { return new Jump(); }

ALIEN_END_HEADER

#endif