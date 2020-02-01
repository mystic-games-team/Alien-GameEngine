#pragma once
#include "SDL/include/SDL_scancode.h"
#include "MathGeoLib/include/Math/float3.h"

enum KEY_STATE;

class __declspec(dllexport) Input {
public:

	enum {
		KEY_IDLE = 0,
		KEY_DOWN,
		KEY_REPEAT,
		KEY_UP
	};

	enum MOUSE_BUTTONS {
		MOUSE_LEFT_BUTTON = 1,
		MOUSE_MIDDLE_BUTTON = 2,
		MOUSE_RIGHT_BUTTON = 3,
	};

	/*----------KEYBOARD--------------*/
	static bool GetKeyDown(const SDL_Scancode& code);
	static bool GetKeyIdle(const SDL_Scancode& code);
	static bool GetKeyUp(const SDL_Scancode& code);
	static bool GetKeyRepeat(const SDL_Scancode& code);
	static SDL_Scancode GetFirstKeyDown();
	/*----------KEYBOARD--------------*/

	/*----------MOUSE--------------*/
	static float3 GetMousePosition();
	static int GetMouseY();
	static int GetMouseX();
	static bool IsAnyMouseButtonPressed();
	static float GetMouseWheel();
	static float GetMouseYMotion();
	static float GetMouseXMotion();
	static bool GetMouseButtonDown(const MOUSE_BUTTONS& button);
	static bool GetMouseButtonUp(const MOUSE_BUTTONS& button);
	static bool GetMouseButtonIdle(const MOUSE_BUTTONS& button);
	static bool GetMouseButtonRepeat(const MOUSE_BUTTONS& button);
	/*----------MOUSE--------------*/

	/*----------CONTROLLER--------------*/
	static float GetControllerHoritzontalLeftAxis(int controller_index);
	/*----------CONTROLLER--------------*/

	// TOOD: mouse pos relative to the panel game texture

private:

	static bool IsInputAvailable();
};
