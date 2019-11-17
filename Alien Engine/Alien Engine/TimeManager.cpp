#include "TimeManager.h"

Time::GameState Time::state = Time::GameState::NONE;
float Time::time_since_start = 0.0F;
float Time::game_time = 0.0F;
float Time::delta_time = 0.0F;
float Time::scale_time = 0.0F;

void Time::Start()
{
	start_timer.Start();
}

void Time::Update()
{
	/*time_since_start = start_timer.ReadSec();

	if (state == GameState::PLAY || state == GameState::PLAY_ONCE) {
		game_time = game_timer.ReadSec();
	}*/
}

void Time::Play()
{
	state = GameState::PLAY;
	game_time = 0.0F;
	//game_timer.Start();
}

void Time::Pause()
{
	state = GameState::PAUSE;
	//game_timer.Pause();
}

void Time::PlayOnce()
{
	//game_timer.Resume();
	state = GameState::PLAY_ONCE;
}
