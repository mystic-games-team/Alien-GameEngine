#pragma once

#include "Primitive.h"

class Sphere_Alien :public Primitive
{
public:

	Sphere_Alien();
	Sphere_Alien(const float& x, const float& y, const float& z, const int& subdivisions);
	~Sphere_Alien();
};