#include "Primitive.h"
#include "Shapes.h"

Primitive::Primitive(const float& position_x, const float& position_y, const float& position_z) : Object(position_x,position_y,position_z)
{
}

Primitive::~Primitive()
{
	par_shapes_free_mesh(shape);
}
