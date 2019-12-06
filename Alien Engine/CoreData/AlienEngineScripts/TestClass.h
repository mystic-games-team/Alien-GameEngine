#ifndef _TESTCLASS_H_
#define _TESTCLASS_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

ALIEN_INIT_HEADER

class ALIEN_ENGINE_API TestClass : public Alien {

public:

	TestClass();

	virtual ~TestClass();


	int classss = 0;
};

ALIEN_INIT_DATA TestClass* CreateTestClass() {
	TestClass* test = new TestClass();
	SHOW_IN_INSPECTOR_AS_INPUT_INT(test->classss)
		return test;
}
ALIEN_DESTROY_DATA void DestroyTestClass(TestClass* test) { delete test; }

ALIEN_END_HEADER

#endif // !_MOVE_H_




