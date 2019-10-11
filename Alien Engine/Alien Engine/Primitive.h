#pragma once

#include "GameObject.h"
#include "Shapes.h"

enum class PrimitiveType
{
	CUBE,
	SPHERE_ALIEN,
	ROCK,
	DODECAHEDRON,
	OCTAHEDRON,
	TORUS,
	ICOSAHEDRON,

	UNKONWN
};

class Primitive : public GameObject
{
public:
	Primitive();
	virtual ~Primitive();

	PrimitiveType type = PrimitiveType::UNKONWN;

	void SetPosition(const float& position_x, const float& position_y, const float& position_z);

	void SetSubdivisions(const int& subdivisions);

protected:

	void InitBuffers();
	void MemCpy();
	void RestartBuffers();

protected:
	par_shapes_mesh* shape = nullptr;
	int subdivisions = 5;

};