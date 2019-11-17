#ifndef __TIME_H__
#define __TIME_H__

class Timer;

class Time {
public:
	friend class Application;

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
	static bool IsPlaying();
	static void Stop();

	static void CleanUp();

	static void SetScaleTime(const float& scale);

	static float GetDT();


public:

	static GameState state;
	static float time_since_start;
	static float game_time;
	static float scale_time;

private:

	static void SetDT(const float& dt);

	static float delta_time;
	static Timer* game_timer;
	static Timer* start_timer;
};

#endif
