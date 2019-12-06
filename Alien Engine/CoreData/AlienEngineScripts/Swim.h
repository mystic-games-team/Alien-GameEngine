#ifndef _SWIM_H_
#define _SWIM_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API SWIM : public Alien {

public:

	SWIM();


	virtual ~SWIM();

	int classss = 0;
};

ALIEN_INIT_DATA SWIM* CreateSWIM() {
	SWIM* test = new SWIM();
	SHOW_IN_INSPECTOR_AS_INPUT_INT(test->classss)
		return test;
}
ALIEN_DESTROY_DATA void DestroySWIM(SWIM* test) { delete test; }

ALIEN_END_HEADER

#endif // !_MOVE_H_









