#ifndef _MOVE_H_
#define _MOVE_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API Move : public Alien {

public:

	Move();

	~Move();

	void Start();

	int test = 0;
};

ALIEN_INIT_DATA Move* CreateMove() {
	Move* move = new Move();
	SHOW_IN_INSPECTOR_AS_INPUT_INT(move->test)
	return move;
} 
ALIEN_DESTROY_DATA void DestroyMove(Move* move) { delete move; }

ALIEN_END_HEADER

#endif // !_MOVE_H_



