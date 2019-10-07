#include "Dodecahedron.h"
#include "Primitive.h"
#include "Shapes.h"

Dodecahedron::Dodecahedron(const float& position_x, const float& position_y, const float& position_z) : Primitive(position_x, position_y, position_z)
{
	shape = par_shapes_create_dodecahedron();
	par_shapes_translate(shape, position_x, position_y, position_z);
	MemCpy();
	InitBuffers();
}

Dodecahedron::~Dodecahedron()
{
}
