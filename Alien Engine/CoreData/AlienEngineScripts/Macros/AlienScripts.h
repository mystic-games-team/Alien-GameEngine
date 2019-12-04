#pragma once

#include "..\..\..\Alien Engine\ComponentScript.h"

#ifdef ALIENENGINESCRIPTS_EXPORTS
#define ALIEN_ENGINE_API __declspec(dllexport)
#else
#define ALIEN_ENGINE_API __declspec(dllimport)
#endif

#define HIDE_IN_INSPECTOR /*HideInInspector*/
#define SHOW_IN_INSPECTOR /*ShowInInspector*/

#define ALIEN_ENGINE_CLASS_API ALIEN_ENGINE_API
#define ALIEN_ENGINE_STRUCT_API ALIEN_ENGINE_API

#define ALIEN_INIT_CLASS ALIEN_ENGINE_API
#define ALIEN_INIT_STRUCT ALIEN_ENGINE_API

#define ALIEN_INIT_HEADER extern "C" {
#define ALIEN_END_HEADER }


#define SHOW_IN_INSPECTOR_AS_INPUT_INT(INT_) ComponentScript::InspectorInputInt(&INT_);

// TODO: Macro SliderInt bla bla inspector typeid(variable) == int go ahead

