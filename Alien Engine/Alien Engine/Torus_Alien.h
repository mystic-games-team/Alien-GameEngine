#pragma once

#include "Primitive.h"

class Torus_Alien :public Primitive
{
public:

	Torus_Alien();
	Torus_Alien(const float& x, const float& y, const float& z, const int& slices, const int& stacks, const float& radius);
	~Torus_Alien();

	void SetParameters(const int& slices, const int& stacks, const float& radius);

	int GetTorusSlices();
	int GetTorusStacks();
	float GetTorusRadius();

private:

	int slices = 10;
	int stacks = 10;
	float radius = 0.5f;
};
