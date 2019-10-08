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

Icosahedron::Icosahedron(const float& x, const float& y, const float& z) : Primitive()
{
	shape = par_shapes_create_icosahedron();
	SetPosition(x, y, z);
	MemCpy();
	InitBuffers();
}

Icosahedron::~Icosahedron()
{
}
