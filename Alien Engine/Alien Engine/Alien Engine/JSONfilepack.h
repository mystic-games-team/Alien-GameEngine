#pragma once

#include "Parson/parson.h"
#include <string>

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

	void SetString(const std::string& name, const std::string& string_parameter);
	const char* GetString(const std::string& name);


private:

	std::string path;
	JSON_Object* object = nullptr;
	JSON_Value* value = nullptr;
	JSON_Value* save_value = nullptr;
	JSON_Object* save_object = nullptr;

};
