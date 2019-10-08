#pragma once

#include "Primitive.h"

class Rock :public Primitive
{
public:

	Rock();
	~Rock();

	void SetSeed(const uint& seed);

public:

	uint seed = 0;
};
