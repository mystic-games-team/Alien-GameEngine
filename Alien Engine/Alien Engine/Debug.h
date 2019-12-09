#pragma once

class __declspec(dllexport) Debug {

public:

	static void Log(const char* message, ...);
	//static void LogToEngine(const char* message, ...);
	// static void Assert();
	// static void ClearDeveloperConsole();

};
