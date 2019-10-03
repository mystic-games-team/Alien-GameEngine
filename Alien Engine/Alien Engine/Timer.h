#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"


class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	unsigned __int32 Read() const;
	float ReadSec() const;

private:
	unsigned __int32	started_at;
};

#endif //__TIMER_H__