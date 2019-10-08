#pragma once

#include "Primitive.h"

class Cube :public Primitive
{
public:
	Cube();
	Cube(const float& x, const float& y, const float& z);
	~Cube();
};