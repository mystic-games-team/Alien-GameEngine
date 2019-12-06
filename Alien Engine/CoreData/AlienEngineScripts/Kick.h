#ifndef _KICK_H_
#define _KICK_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API Kick : public Alien {

public:

	Kick();

	virtual ~Kick();


	int classss = 0;
};

ALIEN_INIT_DATA Kick* CreateKick() {
	Kick* test = new Kick();
	SHOW_IN_INSPECTOR_AS_INPUT_INT(test->classss)
		return test;
}
ALIEN_DESTROY_DATA void DestroyKick(Kick* test) { delete test; }

ALIEN_END_HEADER

#endif // !_MOVE_H_





