#pragma once

#include "Parson/parson.h"
#include <string>
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "SDL/include/SDL_scancode.h"

struct Color;
class JSONArraypack;

class JSONfilepack {

public:

	JSONfilepack(const std::string& path, JSON_Object*& object, JSON_Value*& value);

	~ JSONfilepack();

	void StartSave();
	void FinishSave();
	
	void SetNumber(const std::string& name, const double& number);
	double GetNumber(const std::string& name);

	void SetBoolean(const std::string& name, const bool& boolean);
	bool GetBoolean(const std::string& name);

	void SetColor(const std::string& name, const Color& color);
	Color GetColor(const std::string& name);

	void SetFloat3(const std::string& name, const float3& numbers);
	float3 GetFloat3(const std::string& name);

	void SetQuat(const std::string& name, const Quat& numbers);
	Quat GetQuat(const std::string& name);
	
	void SetNumberArray(const std::string& name, float* numbers, uint size);
	float* GetNumberArray(const std::string& name);

	void SetUintArray(const std::string& name, uint* numbers, uint size);
	uint* GetUintArray(const std::string& name);

	void SetShortcutCodes(const std::string& name, uint codes[3]);
	SDL_Scancode* GetShortcutCodes(const std::string& name);

	void SetArrayString(const std::string& name, std::string* strings, uint size);
	std::string* GetArrayString(const std::string& name);

	void SetString(const std::string& name, const std::string& string_parameter);
	const char* GetString(const std::string& name);

	JSONArraypack* InitNewArray(const std::string& name);

private:

	std::string path;
	JSON_Object* object = nullptr;
	JSON_Value* value = nullptr;
	JSON_Value* save_value = nullptr;
	JSON_Object* save_object = nullptr;

	std::vector<JSONArraypack*> arrays;

};

class JSONArraypack {

public:

	JSONArraypack(JSON_Array* arr, JSON_Value* value) { this->arr = arr; this->value = value; }
	~JSONArraypack();

	void SetNumber(const std::string& name, const double& number);
	double GetNumber(const std::string& name);

	void SetBoolean(const std::string& name, const bool& boolean);
	bool GetBoolean(const std::string& name);

	void SetColor(const std::string& name, const Color& color);
	Color GetColor(const std::string& name);

	void SetFloat3(const std::string& name, const float3& numbers);
	float3 GetFloat3(const std::string& name);

	void SetQuat(const std::string& name, const Quat& numbers);
	Quat GetQuat(const std::string& name);

	void SetAnotherNode();

	void SetString(const std::string& name, const std::string& string_parameter);

	const char* GetString(const std::string& name);

private:

	JSON_Array* arr = nullptr;
	JSON_Value* value = nullptr;

	std::vector<JSONArraypack*> arrays;

};