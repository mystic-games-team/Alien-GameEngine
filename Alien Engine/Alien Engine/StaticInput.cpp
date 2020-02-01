#include "StaticInput.h"
#include "Application.h"
#include "PanelGame.h"

KEY_STATE Input::GetKey(const SDL_Scancode& code)
{
	return App->input->GetKey(code);
}

bool Input::GetKeyDown(const SDL_Scancode& code)
{
	return IsInputAvailable() && App->input->GetKey(code) == KEY_DOWN;
}

bool Input::GetKeyIdle(const SDL_Scancode& code)
{
	return IsInputAvailable() && App->input->GetKey(code) == KEY_IDLE;
}

bool Input::GetKeyUp(const SDL_Scancode& code)
{
	return IsInputAvailable() && App->input->GetKey(code) == KEY_UP;
}

bool Input::GetKeyRepeat(const SDL_Scancode& code)
{
	return IsInputAvailable() && App->input->GetKey(code) == KEY_REPEAT;
}

SDL_Scancode Input::GetFirstKeyDown()
{
	if (IsInputAvailable()) {
		return App->input->GetFirstKeyPressed();
	}
	else {
		return SDL_SCANCODE_UNKNOWN;
	}
}

float3 Input::GetMousePosition()
{
#ifndef GAME_VERSION
	return float3((App->input->GetMouseX() - App->ui->panel_game->posX), (App->input->GetMouseY() - App->ui->panel_game->posY) - 19, App->input->GetMouseZ());
#else
	return App->input->GetMousePosition();
#endif 
}

int Input::GetMouseY()
{
#ifndef GAME_VERSION
	return App->input->GetMouseY() - App->ui->panel_game->posY - 19;
#else
	return App->input->GetMouseY();
#endif
}

int Input::GetMouseX()
{
#ifndef GAME_VERSION
	return App->input->GetMouseX() - App->ui->panel_game->posX;
#else
	return App->input->GetMouseX();
#endif 
}

bool Input::IsAnyMouseButtonPressed()
{
	return IsInputAvailable() && App->input->IsMousePressed();
}

float Input::GetMouseWheel()
{
	return App->input->GetMouseZ();
}

float Input::GetMouseYMotion()
{
	return App->input->GetMouseYMotion();
}

float Input::GetMouseXMotion()
{
	return App->input->GetMouseXMotion();
}

bool Input::GetMouseButtonDown(const MOUSE_BUTTONS& button)
{
	return IsInputAvailable() && App->input->GetMouseButton(button) == KEY_DOWN;
}

bool Input::GetMouseButtonUp(const MOUSE_BUTTONS& button)
{
	return IsInputAvailable() && App->input->GetMouseButton(button) == KEY_UP;
}

bool Input::GetMouseButtonIdle(const MOUSE_BUTTONS& button)
{
	return IsInputAvailable() && App->input->GetMouseButton(button) == KEY_IDLE;
}

bool Input::GetMouseButtonRepeat(const MOUSE_BUTTONS& button)
{
	return IsInputAvailable() && App->input->GetMouseButton(button) == KEY_REPEAT;
}

float Input::GetControllerHoritzontalLeftAxis(int controller_index)
{
	return (!App->input->IsControllerActive(controller_index) ? 0.0f : App->input->game_pads[controller_index]->joystick_left.valueX);
}

float Input::GetControllerVerticalLeftAxis(int controller_index)
{
	return (!App->input->IsControllerActive(controller_index) ? 0.0f : App->input->game_pads[controller_index]->joystick_left.valueY);
}

float Input::GetControllerHoritzontalRightAxis(int controller_index)
{
	return (!App->input->IsControllerActive(controller_index) ? 0.0f : App->input->game_pads[controller_index]->joystick_right.valueX);
}

float Input::GetControllerVerticalRightAxis(int controller_index)
{
	return (!App->input->IsControllerActive(controller_index) ? 0.0f : App->input->game_pads[controller_index]->joystick_right.valueY);
}

KEY_STATE Input::GetControllerButton(int controller_index, const CONTROLLER_BUTTONS& code)
{
	return App->input->GetControllerButton(controller_index, code);
}

bool Input::GetControllerButtonDown(int controller_index, const CONTROLLER_BUTTONS& code)
{
	return App->input->GetControllerButton(controller_index, code) == KEY_DOWN;
}

bool Input::GetControllerButtonIdle(int controller_index, const CONTROLLER_BUTTONS& code)
{
	return App->input->GetControllerButton(controller_index, code) == KEY_IDLE;
}

bool Input::GetControllerButtonUp(int controller_index, const CONTROLLER_BUTTONS& code)
{
	return App->input->GetControllerButton(controller_index, code) == KEY_UP;
}

bool Input::GetControllerButtonRepeat(int controller_index, const CONTROLLER_BUTTONS& code)
{
	return App->input->GetControllerButton(controller_index, code) == KEY_REPEAT;
}

bool Input::IsInputAvailable()
{
#ifndef GAME_VERSION
	return App->ui->panel_game->game_focused;
#else
	return true;
#endif
}
