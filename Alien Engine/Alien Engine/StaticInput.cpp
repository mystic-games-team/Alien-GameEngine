#include "StaticInput.h"
#include "Application.h"

KEY_STATE Input::GetKey(const SDL_Scancode& code)
{
	return App->input->GetKey(code);
}

bool Input::GetKeyDown(const SDL_Scancode& code)
{
	return App->input->GetKey(code) == KEY_DOWN;
}

bool Input::GetKeyIdle(const SDL_Scancode& code)
{
	return App->input->GetKey(code) == KEY_IDLE;
}

bool Input::GetKeyUp(const SDL_Scancode& code)
{
	return App->input->GetKey(code) == KEY_UP;
}

bool Input::GetKeyRepeat(const SDL_Scancode& code)
{
	return App->input->GetKey(code) == KEY_REPEAT;
}

SDL_Scancode Input::GetFirstKeyDown()
{
	return App->input->GetFirstKeyPressed();
}

float3 Input::GetMousePosition()
{
	return App->input->GetMousePosition();
}

float Input::GetMouseY()
{
	return App->input->GetMouseY();
}

float Input::GetMouseX()
{
	return App->input->GetMouseX();
}

bool Input::IsAnyMouseButtonPressed()
{
	return App->input->IsMousePressed();
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

KEY_STATE Input::GetMouseButton(const MOUSE_BUTTONS& button)
{
	return App->input->GetMouseButton(button);
}

bool Input::GetMouseButtonDown(const MOUSE_BUTTONS& button)
{
	return App->input->GetMouseButton(button) == KEY_DOWN;
}

bool Input::GetMouseButtonUp(const MOUSE_BUTTONS& button)
{
	return App->input->GetMouseButton(button) == KEY_UP;
}

bool Input::GetMouseButtonIdle(const MOUSE_BUTTONS& button)
{
	return App->input->GetMouseButton(button) == KEY_IDLE;
}

bool Input::GetMouseButtonRepeat(const MOUSE_BUTTONS& button)
{
	return App->input->GetMouseButton(button) == KEY_REPEAT;
}
