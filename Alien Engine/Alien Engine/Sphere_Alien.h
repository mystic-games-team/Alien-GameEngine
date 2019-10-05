#pragma once

#include "Primitive.h"

class Sphere_Alien :public Primitive
{
public:

	Sphere_Alien(const float& position_x, const float& position_y, const float& position_z, const int &subdivions);
	~Sphere_Alien();
};