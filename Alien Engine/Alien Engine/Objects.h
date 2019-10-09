#pragma once

#include "Globals.h"
#include "glmath.h"
#include "Color.h"

enum class ObjectType
{
	PRIMITIVE,
	CAMERA,

	UNKONWN
};

class Object
{
public:
	Object();
	virtual ~Object();

	void ChangeEnable();
	bool IsEnabled();

	void Draw();

	vec3 GetPosition();

	

protected:
	bool enabled = true;

public:

	ObjectType type = ObjectType::UNKONWN;

	vec3 position = { 0,0,0 };

	uint id_vertex = 0;
	uint id_index = 0;

	float* vertex = nullptr;
	uint* index = nullptr;

	uint num_vertex = 0;
	uint num_index = 0;

	Color color{ 1,1,1 };
};