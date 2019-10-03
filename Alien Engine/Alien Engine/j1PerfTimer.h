#ifndef __j1PERFTIMER_H__
#define __j1PERFTIMER_H__

class j1PerfTimer
{
public:

	// Constructor
	j1PerfTimer();

	void Start();
	double ReadMs() const;
	unsigned __int64 ReadTicks() const;
	double StartedAt();

private:
	unsigned __int64	started_at;
	static unsigned __int64 frequency;
};

#endif //__j1PERFTIMER_H__