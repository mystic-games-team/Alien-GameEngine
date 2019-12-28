#include "Maths.h"
#include "MathGeoLib/include/Math/MathFunc.h"

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

float Maths::Cos(const float& angle_in_degrees)
{
	return math::Cos(angle_in_degrees * Deg2Rad());
}

float Maths::Sin(const float& angle_in_degrees)
{
	return math::Sin(angle_in_degrees * Deg2Rad());
}

float Maths::Power(const float& base, const float& exponent)
{
	if (exponent < 0) 
	{
		if (base == 0)
		{
			return -0; // Error!!
		}
		return 1 / (base * Power(base, (-exponent) - 1));
	}
	if (exponent == 0)
	{
		return 1;
	}
	if (exponent == 1)
	{
		return base;
	}
	return base * Power(base, exponent - 1);
}

int Maths::Factorial(const int& number)
{
	return number <= 0 ? 1 : number * Factorial(number - 1);
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
