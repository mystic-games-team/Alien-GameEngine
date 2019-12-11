#include "b.h"

b::b() : Alien()
{
}

b::~b()
{
}

void b::OnDrawGizmos()
{
	Gizmos::DrawCube(transform->GetGlobalPosition(), { 2,2,2 }, { 0,0,1,1 });
}
