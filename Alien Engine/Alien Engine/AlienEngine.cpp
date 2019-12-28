#include "AlienEngine.h"
#include "Globals.h"
#include "Application.h"

float AlienEngine::GetAlienEngineVersion()
{
	return ALIEN_ENGINE_VERSION;
}

void AlienEngine::OpenURL(const char* website)
{
	ShellExecuteA(NULL, "open", website, NULL, NULL, SW_SHOWNORMAL);
}

void AlienEngine::QuitApp()
{
	App->QuitApp();
}
