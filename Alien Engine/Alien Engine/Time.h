#ifndef __TIME_H__
#define __TIME_H__

class Timer;

class __declspec(dllexport) Time {
private:
	friend class Application;
	friend class ModuleUI;
	friend class ReturnZ;
	friend class ModuleObjects;
	friend class PanelScene;
	friend class PanelInspector;

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
	static bool IsInGameState();
	static void Stop();

	static void CleanUp();

	static void SetDT(const float& dt);

private:

	static float delta_time;
	static Timer* game_timer;
	static Timer* start_timer;

	static GameState state;

	static float time_since_start;
	static float game_time;
	static float scale_time;

public:

	static void SetScaleTime(const float& scale);
	static float GetDT();
	static float GetTimeSinceStart();
	static float GetGameTime();
	static float GetScaleTime();

};

#endif
