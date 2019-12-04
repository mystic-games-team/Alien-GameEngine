#pragma once

#include "Component.h"
#include <vector>
#include <string>

class __declspec(dllexport) ComponentScript : public Component {
public:
	ComponentScript(GameObject* attach);
	virtual ~ComponentScript();

	void Reset();
	void SetComponent(Component* component);

	bool DrawInspector();

	void SaveComponent(JSONArraypack* to_save);
	void LoadComponent(JSONArraypack* to_load);

	static void InspectorInputInt(int* ptr);

public:

	// TODO: change to touple and next option is for personaliing ImGui inspector, like drag int text int bla bla
	// enum for types of imgui to show variables
	// change to struct, need variable name
	std::vector<std::pair<std::string, void*>> inspector_variables;

};
