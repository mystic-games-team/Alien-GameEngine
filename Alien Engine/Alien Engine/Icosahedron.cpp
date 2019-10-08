#include "Icosahedron.h"
#include "Primitive.h"
#include "Shapes.h"

Icosahedron::Icosahedron() : Primitive()
{
	shape = par_shapes_create_icosahedron();
	SetPosition(0, 0, 0);
	MemCpy();
	InitBuffers();
}

Icosahedron::~Icosahedron()
{
}
