#pragma once

#include "..\..\..\Alien Engine\ComponentScript.h"
#include "..\..\..\Alien Engine\ComponentTransform.h"
#include "..\..\..\Alien Engine\MathGeoLib\include\Math\float3.h"
#include "..\..\..\Alien Engine\MathGeoLib\include\Math\Quat.h"

#include "..\..\..\Alien Engine\StaticInput.h"
#include "..\..\..\Alien Engine\Maths.h"
#include "..\..\..\Alien Engine\Debug.h"
#include "..\..\..\Alien Engine\RandomHelper.h"
#include "..\..\..\Alien Engine\Time.h"
#include "..\..\..\Alien Engine\Camera.h"
#include "..\..\..\Alien Engine\Gizmos.h"
#include "..\..\..\Alien Engine\Color.h"

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

// ------------INSPECTOR MACROS----------------
#define SHOW_IN_INSPECTOR_AS_INPUT_INT(INT_) ComponentScript::InspectorInputInt(&INT_, #INT_);
// ------------INSPECTOR MACROS----------------

// TODO: Macro SliderInt bla bla inspector typeid(variable) == int go ahead
