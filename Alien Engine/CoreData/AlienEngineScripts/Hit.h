#ifndef _HIT_H_
#define _HIT_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API Hit : public Alien {

public:

	Hit();

	virtual ~Hit();

	int test = 0;
};

ALIEN_INIT_DATA Hit* CreateHit() {
	Hit* move = new Hit();
	SHOW_IN_INSPECTOR_AS_INPUT_INT(move->test)
		return move;
}
ALIEN_DESTROY_DATA void DestroyHit(Hit* move) { delete move; }

ALIEN_END_HEADER

#endif // !_MOVE_H_




