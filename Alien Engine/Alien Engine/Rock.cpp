#include "Rock.h"
#include "Primitive.h"
#include "Shapes.h"

Rock::Rock(const float& position_x, const float& position_y, const float& position_z, const int &subdivions, const int &extra) :Primitive(position_x, position_y, position_z)
{
	shape = par_shapes_create_rock(extra, subdivions);
	par_shapes_translate(shape, position_x, position_y, position_z);
}

Rock::~Rock()
{
}
