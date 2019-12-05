#pragma once

#include "..\..\..\Alien Engine\ComponentScript.h"

#ifdef ALIENENGINESCRIPTS_EXPORTS
#define ALIEN_ENGINE_API __declspec(dllexport)
#else
#define ALIEN_ENGINE_API __declspec(dllimport)
#endif

// define it next to the CreateClass/StructFunct to be able to use the class/struct
#define ALIEN_INIT_DATA ALIEN_ENGINE_API
#define ALIEN_DESTROY_DATA ALIEN_ENGINE_API

#define ALIEN_INIT_HEADER extern "C" {
#define ALIEN_END_HEADER }

#define SHOW_IN_INSPECTOR_AS_INPUT_INT(INT_) ComponentScript::InspectorInputInt(&INT_, #INT_);

// TODO: Macro SliderInt bla bla inspector typeid(variable) == int go ahead

