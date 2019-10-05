#include "Cube.h"
#include "Primitive.h"
#include "Shapes.h"

Cube::Cube(const float& position_x, const float& position_y, const float& position_z):Primitive(position_x,position_y,position_z)
{
	shape = par_shapes_create_cube();
	par_shapes_translate(shape, position_x, position_y, position_z);
}

Cube::~Cube()
{
}
