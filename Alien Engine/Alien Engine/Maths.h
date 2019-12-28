#pragma once

static class __declspec(dllexport) Maths {
public:

	static int Abs(const int& value);
	static float Abs(const float& value);
	static double Abs(const double& value);

	static int Ceil(const float& value);
	static int Ceil(const double& value);

	static int Floor(const float& value);
	static int Floor(const double& value);

	static int Clamp(const int& value, const int& min, const int& max);
	static float Clamp(const float& value, const float& min, const float& max);
	static double Clamp(const double& value, const double& min, const double& max);

	static float Cos(const float& angle_in_degrees);
	static float Sin(const float& angle_in_degrees);

	static float Power(const float& base, const float& exponent);

	static int Factorial(const int& number);

	static int Min(const int& value1, const int& value2);
	static float Min(const float& value1, const float& value2);
	static double Min(const double& value1, const double& value2);

	static int Max(const int& value1, const int& value2);
	static float Max(const float& value1, const float& value2);
	static double Max(const double& value1, const double& value2);

	// TODO: Lerp(); :D

	static double PI();
	static double Deg2Rad();
	static double Rad2Deg();
};

