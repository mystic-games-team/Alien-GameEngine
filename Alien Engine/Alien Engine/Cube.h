#pragma once

#include "Primitive.h"

class Cube :public Primitive
{
public:

	Cube(const float& position_x, const float& position_y, const float& position_z, const float &scale);
	~Cube();
};