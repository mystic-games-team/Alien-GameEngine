#ifndef _AAAAAAAAAAAA_H_
#define _AAAAAAAAAAAA_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API AAAAAAAAAAAA : public Alien {

public:

	AAAAAAAAAAAA();

	virtual ~AAAAAAAAAAAA();


	int classss = 0;
};

ALIEN_INIT_DATA AAAAAAAAAAAA* CreateAAAAAAAAAAAA() {
	AAAAAAAAAAAA* test = new AAAAAAAAAAAA();
	SHOW_IN_INSPECTOR_AS_INPUT_INT(test->classss)
		return test;
}
ALIEN_DESTROY_DATA void DestroyAAAAAAAAAAAA(AAAAAAAAAAAA* test) { delete test; }

ALIEN_END_HEADER

#endif // !_MOVE_H_





