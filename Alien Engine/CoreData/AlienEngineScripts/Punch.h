#ifndef _PunchL_H_
#define _PunchL_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API Punch : public Alien {

public:

	Punch();

	virtual ~Punch();

	int classss = 0;
};

ALIEN_INIT_DATA Punch* CreatePunch() {
	Punch* test = new Punch();
	SHOW_IN_INSPECTOR_AS_INPUT_INT(test->classss)
		return test;
}
ALIEN_DESTROY_DATA void DestroyPunch(Punch* test) { delete test; }

ALIEN_END_HEADER

#endif // !_MOVE_H_








