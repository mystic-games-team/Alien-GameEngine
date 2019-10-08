#include "Cube.h"
#include "Primitive.h"

Cube::Cube() :Primitive()
{
	shape = par_shapes_create_cube();
	SetPosition(0, 0, 0);
	MemCpy();
	InitBuffers();
}

Cube::~Cube()
{
}
