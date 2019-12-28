#pragma once
#include "Globals.h"
#include "Application.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;
	
	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "%s(%d) : %s", file, line, tmp_string);
	
	if (App != nullptr) {
		if (!App->all_engine_logs.empty()) {
			App->all_engine_logs.appendf(std::string("\n" + std::string(tmp_string2)).data());
		}
		else {
			App->all_engine_logs.appendf(tmp_string2);
		}
		bool exists = false;
		std::vector<LogInfo>::iterator item = App->engine_string_logs.begin();
		for (; item != App->engine_string_logs.end(); ++item) {
			if ((*item).line == line && strcmp((*item).file, file) == 0) {
				exists = true;
				bool is_different = true;
				for (uint i = 0; i < (*item).loged.size(); ++i) {
					if (strcmp((*item).loged[i].second.data(), tmp_string2) == 0) {
						++(*item).loged[i].first;
						is_different = false;
						++(*item).instances;
					}
				}
				if (is_different) {
					(*item).loged.push_back({ 1,tmp_string2 });
					++(*item).instances;
				}
			}
		}
		if (!exists) {
			App->engine_string_logs.push_back(LogInfo(line, file, tmp_string2));
		}
	}
	OutputDebugString(std::string('\n' + tmp_string2).data());
}



