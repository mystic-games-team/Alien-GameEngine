#include "TestMove.h"

TestMove::TestMove() : Alien()
{
}

TestMove::~TestMove()
{
}

void TestMove::Update()
{
	transform->SetLocalPosition(transform->GetLocalPosition() + float3{velocity * Time::GetDT() * Input::GetControllerHoritzontalLeftAxis(1), 0, 0});
}
