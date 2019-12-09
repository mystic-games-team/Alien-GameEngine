#pragma once

class __declspec(dllexport) Debug {

public:

	static void Log(const char* message, ...);

	static void ClearGameConsole();
	static void ClearEngineConsole();

	// static void Assert();

};
