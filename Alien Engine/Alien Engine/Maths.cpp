#include "Maths.h"

float Maths::Abs(const float& value)
{
	return ((value < 0) ? (-value) : (value));
}

double Maths::Abs(const double& value)
{
	return ((value < 0) ? (-value) : (value));
}

int Maths::Abs(const int& value)
{
	return ((value < 0) ? (-value) : (value));
}

int Maths::Ceil(const float& value)
{
	int val = value;
	if ((float)val != value)
		return value + 1;
	return value;
}

int Maths::Ceil(const double& value)
{
	int val = value;
	if ((double)val != value)
		return value + 1;
	return value;
}

int Maths::Floor(const float& value)
{
	int val(value);

	return val - (val > value);
}

int Maths::Floor(const double& value)
{
	int val(value);

	return val - (val > value);
}

int Maths::Clamp(const int& value, const int& min, const int& max)
{
	return (value > max) ? max : ((value < min) ? min : value);
}

float Maths::Clamp(const float& value, const float& min, const float& max)
{
	return (value > max) ? max : ((value < min) ? min : value);
}

double Maths::Clamp(const double& value, const double& min, const double& max)
{
	return (value > max) ? max : ((value < min) ? min : value);
}

int Maths::Min(const int& value1, const int& value2)
{
	return (value1 < value2) ? value1 : value2;
}

float Maths::Min(const float& value1, const float& value2)
{
	return (value1 < value2) ? value1 : value2;
}

double Maths::Min(const double& value1, const double& value2)
{
	return (value1 < value2) ? value1 : value2;
}

int Maths::Max(const int& value1, const int& value2)
{
	return (value1 < value2) ? value2 : value1;
}

float Maths::Max(const float& value1, const float& value2)
{
	return (value1 < value2) ? value2 : value1;
}

double Maths::Max(const double& value1, const double& value2)
{
	return (value1 < value2) ? value2 : value1;
}

double Maths::PI()
{
	return 3.14159265358979f;
}

double Maths::Deg2Rad()
{
	return 0.0174532925199432957f;
}

double Maths::Rad2Deg()
{
	return 57.295779513082320876f;
}
