#ifndef _MOVE_H_
#define _MOVE_H_

#include "..\..\Alien Engine\Alien.h"
#include "Macros/AlienScripts.h"

class ALIEN_ENGINE_API Move : public Alien {

public:

	Move();

	virtual ~Move();


	void Start();
	void Update();
	void OnDrawGizmos();
	int input_int_test = 0;
	int drag_int_test = 1;
	float input_float_test = 0.34F;
	float drag_float_test = 10.4F;
	int slider_int_test = 10;
	float slider_float_test = 10.5F;
	bool jump = false;
	Prefab bullet;
};

ALIEN_FACTORY Move* CreateMove() {
	Move* move = new Move();
	SHOW_IN_INSPECTOR_AS_INPUT_INT(move->input_int_test);
	SHOW_IN_INSPECTOR_AS_DRAGABLE_INT(move->drag_int_test);
	SHOW_IN_INSPECTOR_AS_INPUT_FLOAT(move->input_float_test);
	SHOW_IN_INSPECTOR_AS_DRAGABLE_FLOAT(move->drag_float_test);
	SHOW_IN_INSPECTOR_AS_SLIDER_INT(move->slider_int_test, 0, 20);
	SHOW_IN_INSPECTOR_AS_SLIDER_FLOAT(move->slider_float_test, 1.5F, 13.5F);
	SHOW_IN_INSPECTOR_AS_CHECKBOX_BOOL(move->jump);
	SHOW_IN_INSPECTOR_AS_PREFAB(move->bullet);
	return move;
} 
ALIEN_FACTORY void DestroyMove(Move* move) { delete move; }


#endif // !_MOVE_H_


