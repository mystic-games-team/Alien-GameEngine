#pragma once

class __declspec(dllexport) SceneManager {

	static void LoadScene(const char* scene_name);
	static int ScenesLoaded();

private:

	static int scenes_loaded;
};
