#pragma once

#include "Objects.h"
#include "Shapes.h"

enum class PrimitiveType
{
	CUBE,
	SPHERE_ALIEN,
	ROCK,

	UNKONWN
};

class Primitive : public Object
{
public:
	Primitive(const float& position_x, const float& position_y, const float& position_z);
	virtual ~Primitive();

	PrimitiveType type = PrimitiveType::UNKONWN;

protected:

	void InitBuffers();
	void MemCpy();

protected:
	par_shapes_mesh* shape = nullptr;

};