#pragma once

class __declspec(dllexport) SceneManager {

	void LoadScene(const char* path);
	int ScenesLoaded();

private:

	static int scenes_loaded;
};
