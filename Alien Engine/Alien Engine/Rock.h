#pragma once

#include "Primitive.h"

class Rock :public Primitive
{
public:

	Rock(const float& position_x, const float& position_y, const float& position_z, const int& subdivions, const int& extra);
	~Rock();
};
