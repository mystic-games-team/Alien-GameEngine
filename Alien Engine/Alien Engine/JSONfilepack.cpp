#include "JSONfilepack.h"

JSONfilepack::JSONfilepack(const std::string& path, JSON_Object*& object, JSON_Value*& value)
	: path(path), object(object), value(value)
{
}

JSONfilepack::~JSONfilepack()
{
	json_value_free(value);
}

void JSONfilepack::StartSave()
{
	save_value = json_parse_file(path.data());
	save_object = json_object(save_value);
}

void JSONfilepack::FinishSave()
{
	json_serialize_to_file_pretty(save_value, path.data());
	json_value_free(save_value);
	save_value = nullptr;
	save_object = nullptr;
}

void JSONfilepack::SetNumber(const std::string& name, const double& number)
{
	json_object_dotset_number(save_object, name.data(), number);
}

double JSONfilepack::GetNumber(const std::string& name)
{
	return json_object_dotget_number(object, name.data());
}

void JSONfilepack::SetBoolean(const std::string& name, const bool& boolean)
{
	json_object_dotset_boolean(save_object, name.data(), boolean);
}

bool JSONfilepack::GetBoolean(const std::string& name)
{
	return json_object_dotget_boolean(object, name.data());
}

double JSONfilepack::GetArrayNumber(const std::string& name, const uint& index)
{
	JSON_Array* arr = json_object_dotget_array(object, name.data());
	return json_array_get_number(arr, index);
}

void JSONfilepack::SetString(const std::string& name, const std::string& string_parameter)
{
	json_object_dotset_string(save_object, name.data(), string_parameter.data());
}

const char* JSONfilepack::GetString(const std::string& name)
{
	return json_object_dotget_string(object, name.data());
}
