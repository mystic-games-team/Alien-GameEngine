#ifndef _JUMP_H_
#define _JUMP_H_

#include "Macros/AlienScripts.h"
#include "..\..\Alien Engine\Alien.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API Jump : public Alien {

public:

	Jump();
	~Jump();

	void Start();

};
ALIEN_INIT_DATA Jump* CreateJump() { 
	Jump* jump = new Jump();
	return jump;
}


struct ALIEN_ENGINE_API TestStruct {
	int x = 0;
};
ALIEN_INIT_DATA TestStruct* CreateTestStruct() {
	TestStruct* test_struct = new TestStruct();
	return test_struct;
}

ALIEN_END_HEADER

#endif