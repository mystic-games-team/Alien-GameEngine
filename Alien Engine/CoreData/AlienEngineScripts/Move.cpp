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
	if (Input::GetFirstKeyDown() == SDL_SCANCODE_1)
		test = 12;
	if (Input::GetKey(SDL_SCANCODE_2) == Input::KEY_DOWN)
		test = 13;
	if (Input::GetKeyRepeat(SDL_SCANCODE_3))
		test = 14;
	if (Input::GetMouseButton(Input::MOUSE_LEFT_BUTTON) == Input::KEY_DOWN)
		test = 15;
	Debug::Log("fdsf %i", 10);
}
