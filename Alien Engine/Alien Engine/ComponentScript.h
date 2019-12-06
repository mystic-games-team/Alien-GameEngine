#pragma once

#include "Component.h"
#include <vector>
#include <string>

struct __declspec(dllexport) InspectorScriptData {

	enum DataType { // TODO: what about uint
		INT,
		INT2,
		INT3,
		FLOAT,
		FLOAT2,
		FLOAT3,
		STRING,
	};

	InspectorScriptData(const std::string& variable_name, const DataType& variable_type, void* ptr) {
		this->variable_name = variable_name;
		this->variable_type = variable_type;
		this->ptr = ptr;
	}

	std::string variable_name;
	DataType variable_type;
	void* ptr = nullptr;
};

class __declspec(dllexport) ComponentScript : public Component {
public:
	ComponentScript(GameObject* attach);
	virtual ~ComponentScript();

	void Reset();
	void SetComponent(Component* component);

	bool DrawInspector();

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	static void InspectorInputInt(int* ptr, const char* name);

	void LoadData(const char* name, bool is_alien);

private:

	static std::string GetVariableName(const char* ptr_name);

public:

	u64 resourceID = 0;
	// TODO: change to touple and next option is for personaliing ImGui inspector, like drag int text int bla bla
	// enum for types of imgui to show variables
	// change to struct, need variable name
	std::vector<InspectorScriptData> inspector_variables;
	std::string data_name;
private:

	bool need_alien = false;
	void* data_ptr = nullptr;


};
