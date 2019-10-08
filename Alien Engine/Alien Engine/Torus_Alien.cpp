#include "Torus_Alien.h"
#include "Primitive.h"
#include "Shapes.h"

Torus_Alien::Torus_Alien() : Primitive()
{
	shape = par_shapes_create_torus(slices, stacks, radius);
	SetPosition(0, 0, 0);
	MemCpy();
	InitBuffers();
}

Torus_Alien::Torus_Alien(const float& x, const float& y, const float& z, const int& slices, const int& stacks, const float& radius) : Primitive()
{
	this->slices = slices;
	this->stacks = stacks;
	this->radius = radius;
	shape = par_shapes_create_torus(slices, stacks,radius);
	SetPosition(x, y, z);
	MemCpy();
	InitBuffers();
}

Torus_Alien::~Torus_Alien()
{
}

void Torus_Alien::SetParameters(const int& slices, const int& stacks, const float& radius)
{
	par_shapes_free_mesh(shape);
	RestartBuffers();
	this->slices = slices;
	this->stacks = stacks;
	this->radius = radius;
	shape = par_shapes_create_torus(slices, stacks, radius);
}

int Torus_Alien::GetTorusSlices()
{
	return this->slices;
}

int Torus_Alien::GetTorusStacks()
{
	return this->stacks;
}

float Torus_Alien::GetTorusRadius()
{
	return this->radius;
}