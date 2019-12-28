#include "Color.h"

Color Color::Black()
{
	return Color(0, 0, 0, 1);
}

Color Color::Blue()
{
	return Color(0, 0, 1, 1);
}

Color Color::Clear()
{
	return Color(0, 0, 0, 0);
}

Color Color::Cyan()
{
	return Color(0, 1, 1, 1);
}

Color Color::Gray()
{
	return Color(0.5F, 0.5F, 0.5F, 1);
}

Color Color::Green()
{
	return Color(0, 1, 0, 1);
}

Color Color::Grey()
{
	return Color(0.5F, 0.5F, 0.5F, 1);
}

Color Color::Magenta()
{
	return Color(1, 0, 1, 1);
}

Color Color::Red()
{
	return Color(1, 0, 0, 1);
}

Color Color::White()
{
	return Color(1, 1, 1, 1);
}

Color Color::Yellow()
{
	return Color(1, 0.92F, 0.016F, 1);
}

Color Color::Orange()
{
	return Color(1, 0.5F, 0, 1);
}

Color Color::Purple()
{
	return Color(0.5F, 0, 0.5F, 1);
}
