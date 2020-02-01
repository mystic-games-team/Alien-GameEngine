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

	enum CONTROLLER_BUTTONS {
		CONTROLLER_BUTTON_A,
		CONTROLLER_BUTTON_B,
		CONTROLLER_BUTTON_X,
		CONTROLLER_BUTTON_Y,
		CONTROLLER_BUTTON_BACK,
		CONTROLLER_BUTTON_GUIDE,
		CONTROLLER_BUTTON_START,
		CONTROLLER_BUTTON_LEFTSTICK,
		CONTROLLER_BUTTON_RIGHTSTICK,
		CONTROLLER_BUTTON_LEFTSHOULDER,
		CONTROLLER_BUTTON_RIGHTSHOULDER,
		CONTROLLER_BUTTON_DPAD_UP,
		CONTROLLER_BUTTON_DPAD_DOWN,
		CONTROLLER_BUTTON_DPAD_LEFT,
		CONTROLLER_BUTTON_DPAD_RIGHT,
		CONTROLLER_BUTTON_LEFTTRIGGER,
		CONTROLLER_BUTTON_RIGHTRIGGER
	};

	enum MOUSE_BUTTONS {
		MOUSE_LEFT_BUTTON = 1,
		MOUSE_MIDDLE_BUTTON = 2,
		MOUSE_RIGHT_BUTTON = 3,
	};

	/*----------KEYBOARD--------------*/
	static KEY_STATE GetKey(const SDL_Scancode& code);
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
	static float GetControllerVerticalLeftAxis(int controller_index);
	static float GetControllerHoritzontalRightAxis(int controller_index);
	static float GetControllerVerticalRightAxis(int controller_index);
	static KEY_STATE GetControllerButton(int controller_index, const CONTROLLER_BUTTONS& code);
	static bool GetControllerButtonDown(int controller_index, const CONTROLLER_BUTTONS& code);
	static bool GetControllerButtonIdle(int controller_index, const CONTROLLER_BUTTONS& code);
	static bool GetControllerButtonUp(int controller_index, const CONTROLLER_BUTTONS& code);
	static bool GetControllerButtonRepeat(int controller_index, const CONTROLLER_BUTTONS& code);
	/*----------CONTROLLER--------------*/

private:

	static bool IsInputAvailable();
};
