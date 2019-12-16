#pragma once

#include "..\..\..\Alien Engine\GameObject.h"
/*-----------------COMPONENTS-------------------*/
#include "..\..\..\Alien Engine\ComponentScript.h"
#include "..\..\..\Alien Engine\ComponentTransform.h"
#include "..\..\..\Alien Engine\ComponentCamera.h"
#include "..\..\..\Alien Engine\ComponentLight.h"
#include "..\..\..\Alien Engine\ComponentMaterial.h"
#include "..\..\..\Alien Engine\ComponentMesh.h"
/*-----------------COMPONENTS-------------------*/

/*-----------------MATH-------------------*/
#include "..\..\..\Alien Engine\MathGeoLib\include\Math\float3.h"
#include "..\..\..\Alien Engine\MathGeoLib\include\Math\Quat.h"
#include "..\..\..\Alien Engine\MathGeoLib\include\Math\float4x4.h"
/*-----------------MATH-------------------*/

/*-----------------HELPERS-------------------*/
#include "..\..\..\Alien Engine\StaticInput.h"
#include "..\..\..\Alien Engine\Maths.h"
#include "..\..\..\Alien Engine\Debug.h"
#include "..\..\..\Alien Engine\RandomHelper.h"
#include "..\..\..\Alien Engine\Time.h"
#include "..\..\..\Alien Engine\Camera.h"
#include "..\..\..\Alien Engine\Gizmos.h"
#include "..\..\..\Alien Engine\Color.h"
/*-----------------HELPERS-------------------*/

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

// ------------INSPECTOR MACROS----------------\\
/*--------------------int--------------------*/
#define SHOW_IN_INSPECTOR_AS_INPUT_INT(INT_) ComponentScript::InspectorInputInt(&INT_, #INT_)
#define SHOW_IN_INSPECTOR_AS_DRAGABLE_INT(INT_) ComponentScript::InspectorDragableInt(&INT_, #INT_)
#define SHOW_IN_INSPECTOR_AS_SLIDER_INT(INT_, MIN_, MAX_) ComponentScript::InspectorSliderInt(&INT_, #INT_, MIN_, MAX_)
/*--------------------float--------------------*/
#define SHOW_IN_INSPECTOR_AS_INPUT_FLOAT(FLOAT_) ComponentScript::InspectorInputFloat(&FLOAT_, #FLOAT_)
#define SHOW_IN_INSPECTOR_AS_DRAGABLE_FLOAT(FLOAT_) ComponentScript::InspectorDragableFloat(&FLOAT_, #FLOAT_)
#define SHOW_IN_INSPECTOR_AS_SLIDER_FLOAT(FLOAT_, MIN_, MAX_) ComponentScript::InspectorSliderFloat(&FLOAT_, #FLOAT_, MIN_, MAX_)
/*--------------------bool--------------------*/
#define SHOW_IN_INSPECTOR_AS_CHECKBOX_BOOL(BOOL_) ComponentScript::InspectorBool(&BOOL_, #BOOL_)

#define SHOW_IN_INSPECTOR_AS_ENUM(ENUM_, ENUM_CLASS) ComponentScript::InspectorEnum<ENUM_CLASS>(&ENUM_, #ENUM_)

// ------------INSPECTOR MACROS----------------\\

// TODO: Macro SliderInt bla bla inspector typeid(variable) == int go ahead
