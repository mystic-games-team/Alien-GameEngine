#pragma once

#include "Primitive.h"

class Octahedron :public Primitive
{
public:

	Octahedron();
	Octahedron(const float& x, const float& y, const float& z);
	~Octahedron();
};