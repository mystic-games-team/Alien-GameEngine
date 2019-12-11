#ifndef _MOVE_H_
#define _MOVE_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API c : public Alien {

public:

	c();

	virtual ~c();




};

ALIEN_INIT_DATA c* Createc() {
	c* move = new c();

		return move;
}
ALIEN_DESTROY_DATA void Destroyc(c* move) { delete move; }

ALIEN_END_HEADER

#endif // !_MOVE_H_


