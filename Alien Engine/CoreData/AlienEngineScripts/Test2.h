#ifndef _TESTCLASS2_H_
#define _TESTCLASS2_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API TestClass2 : public Alien {

public:

	TestClass2();

	virtual ~TestClass2();


	int classss = 0;
};

ALIEN_INIT_DATA TestClass2* CreateTestClass2() {
	TestClass2* test = new TestClass2();
	SHOW_IN_INSPECTOR_AS_INPUT_INT(test->classss)
		return test;
}
ALIEN_DESTROY_DATA void DestroyTestClass2(TestClass2* test) { delete test; }

ALIEN_END_HEADER

#endif // !_MOVE_H_





