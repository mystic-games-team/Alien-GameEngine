#pragma once

#include "Primitive.h"

class Torus_Alien :public Primitive
{
public:

	Torus_Alien();
	~Torus_Alien();

	void SetParameters(par_shapes_mesh* mesh, const int& slices, const int& stacks, const float& radius);

	int GetTorusSlices(par_shapes_mesh* mesh);
	int GetTorusStacks(par_shapes_mesh* mesh);
	float GetTorusRadius(par_shapes_mesh* mesh);

private:

	int slices = 10;
	int stacks = 10;
	float radius = 0.5f;
};
