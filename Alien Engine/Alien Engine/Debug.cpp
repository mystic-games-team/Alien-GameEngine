#include "Debug.h"
#include "Globals.h"
#include "Application.h"

void Debug::Log(const char* message, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;
	// Construct the string from variable arguments
	va_start(ap, message);
	vsprintf_s(tmp_string, 4096, message, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s", tmp_string);
	OutputDebugString(tmp_string2);
	if (App != nullptr)
		App->game_logs.appendf(tmp_string2);
}

void Debug::ClearGameConsole()
{
	App->game_logs.clear();
}

void Debug::ClearEngineConsole()
{
	App->engine_logs.clear();
}
