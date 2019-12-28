#pragma once

class __declspec(dllexport) Screen {
public:
	static bool IsFullScreen();
	static void SetFullScreen(bool fullscreen);
	static bool IsFullDesktop();
	static void SetFullDesktop(bool fulldesktop);
	static bool IsResizable();
	static void SetResizable(bool resizable);
	static bool IsBorderless();
	static void SetBorderless(bool borderless);
	static int GetHeight();
	static int GetWidth();
};
