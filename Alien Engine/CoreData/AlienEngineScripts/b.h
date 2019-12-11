#ifndef _MOVE_H_
#define _MOVE_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API b : public Alien {

public:

	b();

	virtual ~b();


};

ALIEN_INIT_DATA b* Createb() {
	b* move = new b();

		return move;
}
ALIEN_DESTROY_DATA void Destroyb(b* move) { delete move; }

ALIEN_END_HEADER

#endif // !_MOVE_H_



