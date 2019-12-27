#pragma once

class __declspec(dllexport) SceneManager {

	// TODO: change path to name and adapt it to find the resource with x name and pass de metadata path
	static void LoadScene(const char* scene_name);
	static int ScenesLoaded();

private:

	static int scenes_loaded;
};
