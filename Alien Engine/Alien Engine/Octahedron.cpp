#include "Octahedron.h"
#include "Primitive.h"
#include "Shapes.h"

Octahedron::Octahedron(const float& position_x, const float& position_y, const float& position_z) : Primitive(position_x, position_y, position_z)
{
	shape = par_shapes_create_octahedron();
	par_shapes_translate(shape, position_x, position_y, position_z);
	MemCpy();
	InitBuffers();
}

Octahedron::~Octahedron()
{
}
