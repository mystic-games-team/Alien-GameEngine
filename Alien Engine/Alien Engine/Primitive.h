#pragma once

#include "Objects.h"

class Primitive : public Object
{
	enum class PrimitiveType
	{
		CUBE,
		SPHERE,
		ROCK,

		UNKONWN
	};

public:
	Primitive(float position_x, float position_y, float position_z);
	virtual ~Primitive();


};