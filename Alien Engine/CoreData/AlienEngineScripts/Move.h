#ifndef _MOVE_H_
#define _MOVE_H_

#include "..\..\Alien Engine\Alien.h"
#include "AlienScripts.h"

#include <iostream>
#include <utility>

ALIEN_INIT_HEADER

class ALIEN_ENGINE_CLASS_API Move : Alien {

public:

	Move();
	~Move();

	void Start();

	int num = 0;
};
ALIEN_INIT_CLASS Move* CreateMove() {
	Move* move = new Move();
	ComponentScript::LoadInspector(&move->num);
	return move;
} 

ALIEN_END_HEADER

#endif // !_MOVE_H_



