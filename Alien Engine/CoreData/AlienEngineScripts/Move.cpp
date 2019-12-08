#include "Move.h"

Move::Move() : Alien()
{

}

Move::~Move()
{

}

void Move::Start()
{

}

void Move::Update()
{
	test = 12;
	float3 pos = { 10,-10,5 };
	transform->SetLocalPosition(pos);
	pos.FromString("fsadf");

	//Input::GetKey(SDL_SCANCODE_A);
	float pi = Maths::PI();
	//float pi = Maths::PI;

}
