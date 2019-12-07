#ifndef _PULL_H_
#define _PULL_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API PULL : public Alien {

public:

	PULL();

	virtual ~PULL();


	int classss = 0;
};

ALIEN_INIT_DATA PULL* CreatePULL() {
	PULL* test = new PULL();
		return test;
}
ALIEN_DESTROY_DATA void DestroyPULL(PULL* test) { delete test; }

ALIEN_END_HEADER

#endif // !_MOVE_H_






