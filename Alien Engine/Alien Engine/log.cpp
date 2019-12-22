#pragma once
#include "Globals.h"
#include "Application.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;
	bool exists = false;
	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	

	sprintf_s(tmp_string2, 4096, "%s(%d) : %s", file, line, tmp_string);

	if (App != nullptr) {
		auto item = App->engine_string_logs.begin();
		for (; item != App->engine_string_logs.end(); ++item) {
			if (strcmp((*item).second.data(), tmp_string2) == 0) {
				++(*item).first;
				exists = true;
				break;
			}
		}
		if (!exists) {
			App->engine_string_logs.push_back({ 1,tmp_string2 });
		}
	}
	OutputDebugString(std::string('\n' + tmp_string2).data());
}



