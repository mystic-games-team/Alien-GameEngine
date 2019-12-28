
#pragma once

class __declspec(dllexport) Color
{
public:
	
	Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

	Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}

	void Set(float r, float g, float b, float a = 1.0f) 
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	float* operator & ()
	{
		return (float*)this;
	}

	static Color Black();
	static Color Blue();
	static Color Clear();
	static Color Cyan();
	static Color Gray();
	static Color Green();
	static Color Grey();
	static Color Magenta();
	static Color Red();
	static Color White();
	static Color Yellow();
	static Color Orange();
	static Color Purple();


public:
	float r, g, b, a;
};