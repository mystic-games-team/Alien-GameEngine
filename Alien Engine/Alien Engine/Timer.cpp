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
	offset = 0;
	is_pause = false;
}

// ---------------------------------------------
unsigned __int32 Timer::Read() const
{
	return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
float Timer::ReadSec() const
{
	if (!is_pause)
		return float(SDL_GetTicks() - started_at - offset) / 1000.0f;
	else
		return (float)(SDL_GetTicks() - started_at - (SDL_GetTicks() - offset)) / 1000.0f;
}

void Timer::Pause()
{
	is_pause = true;
	offset = SDL_GetTicks();
}

void Timer::Resume()
{
	is_pause = false;
	offset = SDL_GetTicks() - offset;
}
