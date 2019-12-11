#pragma once

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API Fix : public Alien {

public:

	Fix();

	virtual ~Fix();


	void Start();
	void Update();

	int test = 0;

};

ALIEN_INIT_DATA Fix* CreateFix() {
	Fix* move = new Fix();
	SHOW_IN_INSPECTOR_AS_INPUT_INT(move->test)
		return move;
}
ALIEN_DESTROY_DATA void DestroyFix(Fix* move) { delete move; }

ALIEN_END_HEADER

