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
	void Pause();
	void Resume();

private:
	unsigned __int32	started_at;
	unsigned __int32	offset = 0;
	bool is_pause = false;
};

#endif //__TIMER_H__