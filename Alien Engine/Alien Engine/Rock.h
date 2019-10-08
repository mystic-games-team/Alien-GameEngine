#pragma once

#include "Primitive.h"

class Rock :public Primitive
{
public:

	Rock();
	Rock(const float& x, const float& y, const float& z, const uint& seed, const int& subdivisions);
	~Rock();

	void SetSeed(const uint& seed);

public:

	uint seed = 0;
};
