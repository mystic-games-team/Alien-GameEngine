// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
Timer::Timer()
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
unsigned __int32 Timer::Read() const
{
	return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
float Timer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}