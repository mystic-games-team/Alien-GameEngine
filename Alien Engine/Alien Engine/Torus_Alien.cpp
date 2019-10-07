#include "Torus_Alien.h"
#include "Primitive.h"
#include "Shapes.h"

Torus_Alien::Torus_Alien(const float& position_x, const float& position_y, const float& position_z, const int& slices, const int& slacks, const float& radious) : Primitive(position_x, position_y, position_z)
{
	shape = par_shapes_create_torus(slices, slacks, radious);
	par_shapes_translate(shape, position_x, position_y, position_z);
	MemCpy();
	InitBuffers();
}

Torus_Alien::~Torus_Alien()
{
}
