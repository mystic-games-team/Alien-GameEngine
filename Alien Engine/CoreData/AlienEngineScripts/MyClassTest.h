#pragma once

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API MyClassTest : public Alien {

public:

	MyClassTest();

	virtual ~MyClassTest();

};

ALIEN_INIT_DATA MyClassTest* CreateMyClassTest() {
	MyClassTest* move = new MyClassTest();

		return move;
}
ALIEN_DESTROY_DATA void DestroyMyClassTest(MyClassTest* move) { delete move; }

ALIEN_END_HEADER


