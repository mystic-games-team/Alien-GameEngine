#include "Dodecahedron.h"
#include "Primitive.h"

Dodecahedron::Dodecahedron() : Primitive()
{
	shape = par_shapes_create_dodecahedron();
	SetPosition(0, 0, 0);
	MemCpy();
	InitBuffers();
}

Dodecahedron::~Dodecahedron()
{
}
