#include "StaticInput.h"
#include "Application.h"

KEY_STATE Input::GetKey(const SDL_Scancode& code)
{
	return App->input->GetKey(code);
}
