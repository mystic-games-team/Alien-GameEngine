#pragma once

#include "Objects.h"

class Model3D : public Object
{
public:
	Model3D();
	virtual ~Model3D();



public:

	const char* path = nullptr;
};