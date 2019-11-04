#pragma once

#include "Parson/parson.h"
#include <string>
#include "Globals.h"

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

	void SetArrayNumber(const std::string& name, const double& number);
	double GetArrayNumber(const std::string& name, const uint& index);

	void SetArrayString(const std::string& name, const std::string& string_parameter);
	const char* GetArrayString(const std::string& name, const uint& index);

	void SetString(const std::string& name, const std::string& string_parameter);
	const char* GetString(const std::string& name);

	JSON_Array* InitNewArray(const std::string& name);

private:

	std::string path;
	JSON_Object* object = nullptr;
	JSON_Value* value = nullptr;
	JSON_Value* save_value = nullptr;
	JSON_Object* save_object = nullptr;

};
