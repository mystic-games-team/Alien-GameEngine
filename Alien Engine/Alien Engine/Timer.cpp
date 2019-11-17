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
		return float((SDL_GetTicks() - started_at - offset) / 1000.0f);
	else
		return time_passed;
}

void Timer::Pause()
{
	time_passed = float((SDL_GetTicks() - started_at - offset) / 1000.0f);
	is_pause = true;
	time_pause = SDL_GetTicks();
}

void Timer::Resume()
{
	unsigned __int32 time = SDL_GetTicks();
	offset = time - time_pause + offset;
	is_pause = false;
}
