#include "Primitive.h"
#include "Shapes.h"
#include "Sphere_Alien.h"

Sphere_Alien::Sphere_Alien(const float& position_x, const float& position_y, const float& position_z, const int &subdivions) :Primitive(position_x, position_y, position_z)
{
	shape = par_shapes_create_subdivided_sphere(subdivions);
	par_shapes_translate(shape, position_x, position_y, position_z);
}

Sphere_Alien::~Sphere_Alien()
{
}
