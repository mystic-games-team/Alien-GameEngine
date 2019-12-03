#include "AlienScripts.h"
#include "..\..\Alien Engine\GameObject.h"
#include "..\..\Alien Engine\StaticInput.h"
const char* LogToConsole()
{
	//LOG("AAAAAAAAAAAAAAAA");
	return "AAAAAAAAA";
}

void DeleteGameObject(GameObject* obj)
{
	if (Input::GetKey(SDL_SCANCODE_D) == Input::KEY_REPEAT) {
		if (obj != nullptr) {
			obj->ToDelete();
		}
	}
}