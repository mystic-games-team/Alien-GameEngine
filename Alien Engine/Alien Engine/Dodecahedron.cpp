#include "Dodecahedron.h"
#include "Primitive.h"

Dodecahedron::Dodecahedron() : Primitive()
{
	shape = par_shapes_create_dodecahedron();
	SetPosition(0, 0, 0);
	MemCpy();
	InitBuffers();
}

Dodecahedron::Dodecahedron(const float& x, const float& y, const float& z) : Primitive()
{
	shape = par_shapes_create_dodecahedron();
	SetPosition(x, y, z);
	MemCpy();
	InitBuffers();
}

Dodecahedron::~Dodecahedron()
{
}

