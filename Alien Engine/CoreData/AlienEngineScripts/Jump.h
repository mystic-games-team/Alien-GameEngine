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
ALIEN_DESTROY_DATA void DestroyJump(Jump* jump) { delete jump; }

struct ALIEN_ENGINE_API TestStruct {
	TestStruct();
	~TestStruct();
	int x = 0;
};
ALIEN_INIT_DATA TestStruct* CreateTestStruct() {
	TestStruct* test_struct = new TestStruct();
	return test_struct;
}
ALIEN_DESTROY_DATA void DestroyTestStruct(TestStruct* stru) { delete stru; }

ALIEN_END_HEADER

#endif