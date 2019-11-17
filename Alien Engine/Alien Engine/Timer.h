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
	float ReadSec(float scale);
	void Pause();
	void Resume();

private:
	unsigned __int32	started_at;
	unsigned __int32	time_pause;
	unsigned __int32	offset = 0;
	unsigned __int32	time_passed = 0;
	float	curr_time = 0;
	float	time_last_update = 0;
	bool is_pause = false;
};

#endif //__TIMER_H__