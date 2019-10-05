#pragma once

#include "Globals.h"
#include "glmath.h"

enum class ObjectType
{
	PRIMITIVE,
	CAMERA,

	UNKONWN
};

class Object
{
public:
	Object(const float& position_x, const float& position_y, const float& position_z);
	virtual ~Object();

	void ChangeEnable();
	bool IsEnabled();

	vec3 GetPosition();

protected:
	bool enabled = true;

public:

	ObjectType type = ObjectType::UNKONWN;

	vec3 position = { 0,0,0 };

	uint iter_id = 0;
	uint iter_index = 0;
};