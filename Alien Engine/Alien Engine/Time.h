#ifndef __TIME_H__
#define __TIME_H__

class Timer;

class Time {
public:

	enum class GameState {
		NONE,
		PLAY,
		PAUSE,
		PLAY_ONCE
	};

	static void Start();
	static void Update();
	static void Play();
	static void Pause();
	static void PlayOnce();
	static void CleanUp();

public:

	static GameState state;
	static float time_since_start;
	static float game_time;
	static float delta_time;
	static float scale_time;

private:

	static Timer* game_timer;
	static Timer* start_timer;
};

#endif
