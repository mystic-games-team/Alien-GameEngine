#pragma once

#include "..\..\..\Alien Engine\ComponentScript.h"

#ifdef ALIENENGINESCRIPTS_EXPORTS
#define ALIEN_ENGINE_API __declspec(dllexport)
#else
#define ALIEN_ENGINE_API __declspec(dllimport)
#endif

// if this macro names ara changed, it is needed to update CreateMetaData of ResourceScript

// defining a class with this macro will convertit as a possible class component
#define ALIEN_ENGINE_CLASS_API ALIEN_ENGINE_API

// defining a class with this macro will convertit as a possible struct component
#define ALIEN_ENGINE_STRUCT_API ALIEN_ENGINE_API

// define it next to the CreateClassFunct to be able to use the class
#define ALIEN_INIT_CLASS ALIEN_ENGINE_API

// define it next to the CreateStructFunct to be able to use the struct
#define ALIEN_INIT_STRUCT ALIEN_ENGINE_API

#define ALIEN_INIT_HEADER extern "C" {
#define ALIEN_END_HEADER }

#define SHOW_IN_INSPECTOR_AS_INPUT_INT(INT_) ComponentScript::InspectorInputInt(&INT_);

// TODO: Macro SliderInt bla bla inspector typeid(variable) == int go ahead

