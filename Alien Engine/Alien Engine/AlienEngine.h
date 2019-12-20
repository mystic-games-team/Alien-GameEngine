#pragma once

class __declspec(dllexport) AlienEngine {
	static float GetAlienEngineVersion(); 
	static void OpenURL(const char* website);
	static void QuitApp();
};
