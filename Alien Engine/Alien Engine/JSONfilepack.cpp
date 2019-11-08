#include "JSONfilepack.h"
#include "Color.h"


JSONfilepack::JSONfilepack(const std::string& path, JSON_Object*& object, JSON_Value*& value)
	: path(path), object(object), value(value)
{
}

JSONfilepack::~JSONfilepack()
{
	if (value != nullptr)
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

void JSONfilepack::SetColor(const std::string& name, const Color& color)
{
	JSON_Array* arr = json_object_dotget_array(save_object, name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(save_object, name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	json_array_append_number(arr, color.r);
	json_array_append_number(arr, color.g);
	json_array_append_number(arr, color.b);
	json_array_append_number(arr, color.a);
}

Color JSONfilepack::GetColor(const std::string& name)
{
	JSON_Array* arr = json_object_dotget_array(object, name.data());

	Color color;
	color.r = json_array_get_number(arr, 0);
	color.g = json_array_get_number(arr, 1);
	color.b = json_array_get_number(arr, 2);
	color.a = json_array_get_number(arr, 3);

	return color;
}

void JSONfilepack::SetFloat3(const std::string& name, const float3& numbers)
{
	JSON_Array* arr = json_object_dotget_array(save_object, name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(save_object, name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	json_array_append_number(arr, numbers.x);
	json_array_append_number(arr, numbers.y);
	json_array_append_number(arr, numbers.z);
}

float3 JSONfilepack::GetFloat3(const std::string& name)
{
	JSON_Array* arr = json_object_dotget_array(object, name.data());

	float3 numbers;
	numbers.x = json_array_get_number(arr, 0);
	numbers.y = json_array_get_number(arr, 1);
	numbers.z = json_array_get_number(arr, 2);

	return numbers;
}

void JSONfilepack::SetQuat(const std::string& name, const Quat& numbers)
{
	JSON_Array* arr = json_object_dotget_array(save_object, name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(save_object, name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	json_array_append_number(arr, numbers.x);
	json_array_append_number(arr, numbers.y);
	json_array_append_number(arr, numbers.z);
	json_array_append_number(arr, numbers.w);
}

Quat JSONfilepack::GetQuat(const std::string& name)
{
	JSON_Array* arr = json_object_dotget_array(object, name.data());

	Quat quat;
	quat.x = json_array_get_number(arr, 0);
	quat.y = json_array_get_number(arr, 1);
	quat.z = json_array_get_number(arr, 2);
	quat.w = json_array_get_number(arr, 3);

	return quat;
}

void JSONfilepack::SetNumberArray(const std::string& name, float* numbers, uint size)
{
	JSON_Array* arr = json_object_dotget_array(save_object, name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(save_object, name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	for (uint i = 0; i < size; ++i) {
		json_array_append_number(arr, numbers[i]);
	}
}

float* JSONfilepack::GetNumberArray(const std::string& name)
{
	JSON_Array* arr = json_object_dotget_array(object, name.data());
	
	uint size = json_array_get_count(arr);

	float* numbers = new float[size];
	
	for (uint i = 0; i < size; ++i) {
		numbers[i] = json_array_get_number(arr, i);
	}
	
	return numbers;
}

void JSONfilepack::SetUintArray(const std::string& name, uint* numbers, uint size)
{
	JSON_Array* arr = json_object_dotget_array(save_object, name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(save_object, name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	for (uint i = 0; i < size; ++i) {
		json_array_append_number(arr, numbers[i]);
	}
}

uint* JSONfilepack::GetUintArray(const std::string& name)
{
	JSON_Array* arr = json_object_dotget_array(object, name.data());

	uint size = json_array_get_count(arr);

	uint* numbers = new uint[size];

	for (uint i = 0; i < size; ++i) {
		numbers[i] = json_array_get_number(arr, i);
	}

	return numbers;
}

void JSONfilepack::SetShortcutCodes(const std::string& name, uint codes[3])
{
	JSON_Array* arr = json_object_dotget_array(save_object, name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(save_object, name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	for (uint i = 0; i < 3; ++i) {
		json_array_append_number(arr, codes[i]);
	}
}

SDL_Scancode* JSONfilepack::GetShortcutCodes(const std::string& name)
{
	JSON_Array* arr = json_object_dotget_array(object, name.data());

	uint size = json_array_get_count(arr);

	SDL_Scancode codes[3];

	for (uint i = 0; i < 3; ++i) {
		codes[i] = (SDL_Scancode)(uint)json_array_get_number(arr, i);
	}

	return codes;
}

void JSONfilepack::SetArrayString(const std::string& name, std::string* strings, uint size)
{
	JSON_Array* arr = json_object_dotget_array(save_object, name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(save_object, name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	for (uint i = 0; i < size; ++i) {
		json_array_append_string(arr, strings[i].data());
	}
}

std::string* JSONfilepack::GetArrayString(const std::string& name)
{
	JSON_Array* arr = json_object_dotget_array(object, name.data());

	uint size = json_array_get_count(arr);

	std::string* strings = new std::string[size];

	for (uint i = 0; i < size; ++i) {
		strings[i] = json_array_get_string(arr, i);
	}

	return strings;
}

void JSONfilepack::SetString(const std::string& name, const std::string& string_parameter)
{
	json_object_dotset_string(save_object, name.data(), string_parameter.data());
}

const char* JSONfilepack::GetString(const std::string& name)
{
	return json_object_dotget_string(object, name.data());
}

JSON_Array* JSONfilepack::InitNewArray(const std::string& name)
{
	//json_object_dotset_value(save_object, name.data(), json_value_init_array());


	//json_array_append_number(json_object_get_array(save_object, name.data()), 10);
	//json_object_dotset_value(json_array_get_object(json_object_get_array(save_object, name.data()), 0), "attach", json_array_get_value(json_object_get_array(save_object, name.data()), 0));
	JSON_Value* val = json_value_init_array();
	json_object_dotset_value(save_object, name.data(), val);
	JSON_Array* arr = json_value_get_array(val);

	JSON_Object* obj = json_array_get_object(arr, 0);

	JSON_Value* new_val = json_value_init_object();
	json_object_dotset_number(json_value_get_object(new_val), "test", 10);

	json_array_append_value(arr, new_val);

	return arr;
}

void JSONfilepack::SetNumberInArray(const std::string& name, const double& number, JSON_Array* arr)
{
	json_array_append_number(arr, number);
}

void JSONfilepack::AttachObjectToArray(JSON_Object* obj, JSON_Array* arr) 
{

}
