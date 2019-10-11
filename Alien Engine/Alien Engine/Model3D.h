#pragma once

#include "GameObject.h"

class Model3D : public GameObject
{
public:
	Model3D();
	virtual ~Model3D();



public:

	const char* path = nullptr;
};