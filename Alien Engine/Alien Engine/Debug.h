#pragma once

class __declspec(dllexport) Debug {

public:

	static void Log(const char file[], int line, const char* format, ...);

	static void ClearGameConsole();
	static void ClearEngineConsole();

	// static void Assert();

};
