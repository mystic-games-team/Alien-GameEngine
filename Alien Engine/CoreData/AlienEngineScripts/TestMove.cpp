#include "TestMove.h"

TestMove::TestMove() : Alien()
{
}

TestMove::~TestMove()
{
}

void TestMove::Update()
{
	
	X = Input::GetControllerHoritzontalLeftAxis(1);
	Y = Input::GetControllerVerticalLeftAxis(1); 
	if (Input::GetControllerButtonRepeat(1, Input::CONTROLLER_BUTTONS::CONTROLLER_BUTTON_A)) {
		X = 1;
	}
	if (Input::GetControllerButtonRepeat(1, Input::CONTROLLER_BUTTONS::CONTROLLER_BUTTON_Y)) {
		X = -1;
	}
	if (Input::GetControllerButtonRepeat(1, Input::CONTROLLER_BUTTONS::CONTROLLER_BUTTON_DPAD_DOWN)) {
		Y = -1;
	}
	if (Input::GetControllerButtonRepeat(1, Input::CONTROLLER_BUTTONS::CONTROLLER_BUTTON_LEFTTRIGGER)) {
		Y = 1;
	}
	transform->SetLocalPosition(transform->GetLocalPosition() + float3{velocity * Time::GetDT() * X, 0, velocity * Time::GetDT() * Y });

}
