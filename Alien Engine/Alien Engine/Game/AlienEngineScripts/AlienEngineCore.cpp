#include "pch.h"
#include "AlienEngineCore.h"
#include "GameObject.h"

const char* LogToConsole()
{
	//LOG("AAAAAAAAAAAAAAAA");
	return "AAAAAAAAA";
}

void DeleteGameObject(GameObject* obj)
{
	if (obj != nullptr) {
		obj->ToDelete();
	}
}
