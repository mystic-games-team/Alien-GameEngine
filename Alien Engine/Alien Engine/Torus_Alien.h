#pragma once

#include "Primitive.h"

class Torus_Alien :public Primitive
{
public:

	Torus_Alien(const float& position_x, const float& position_y, const float& position_z, const int& slices, const int& slacks, const float& radious);
	~Torus_Alien();
};
