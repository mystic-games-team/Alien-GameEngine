#ifndef _MOVE_H_
#define _MOVE_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API a : public Alien {

public:

	a();

	virtual ~a();

};

ALIEN_INIT_DATA a* Createa() {
	a* move = new a();
		return move;
}
ALIEN_DESTROY_DATA void Destroya(a* move) { delete move; }

ALIEN_END_HEADER

#endif // !_MOVE_H_



