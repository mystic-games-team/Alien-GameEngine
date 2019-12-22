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
	sprintf_s(tmp_string2, 4096, "%s", tmp_string);

	bool exists = false;
	
	/*auto item = App->game_string_logs.begin();
	for (; item != App->game_string_logs.end(); ++item) {
		if ((*item).line == line && strcmp((*item).file, file)) {
			exists = true;
			(*item).loged = tmp_string2;
			++(*item).instances;
		}
	}
	if (!exists) {
		App->game_string_logs.push_back(LogInfo(line, file, tmp_string2));
	}*/

	OutputDebugString(std::string('\n' + tmp_string2).data());

}

void Debug::ClearGameConsole()
{
	App->game_string_logs.clear();
}

void Debug::ClearEngineConsole()
{
	App->engine_string_logs.clear();
}
