#pragma once

#include "glmath.h"

class Object
{
public:
	Object(float position_x, float position_y, float position_z);
	virtual ~Object();

	void ChangeEnable();
	bool IsEnabled();

	vec3 GetPosition();

protected:
	bool enabled = true;

private:
	vec3 position = { 0,0,0 };

};