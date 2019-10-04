#pragma once

#include "Objects.h"
#include "Shapes.h"

enum class PrimitiveType
{
	CUBE,
	SPHERE,
	ROCK,

	UNKONWN
};

class Primitive : public Object
{
public:
	Primitive(const float& position_x, const float& position_y, const float& position_z, const float& scale);
	virtual ~Primitive();

	PrimitiveType type = PrimitiveType::UNKONWN;

public:
	par_shapes_mesh* shape=nullptr;

};