#include "Objects.h"

Object::Object(float position_x, float position_y, float position_z)
{
	position.Set(position_x, position_y, position_z);
}

Object::~Object()
{
}

void Object::ChangeEnable()
{
	enabled = !enabled;
}

bool Object::IsEnabled()
{
	return enabled;
}

vec3 Object::GetPosition()
{
	return position;
}
