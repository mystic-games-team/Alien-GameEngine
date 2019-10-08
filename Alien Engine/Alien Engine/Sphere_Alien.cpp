#include "Primitive.h"
#include "Shapes.h"
#include "Sphere_Alien.h"

Sphere_Alien::Sphere_Alien() : Primitive()
{
	shape = par_shapes_create_subdivided_sphere(subdivisions);
	SetPosition(0, 0, 0);
	MemCpy();
	InitBuffers();
}

Sphere_Alien::~Sphere_Alien()
{
}
