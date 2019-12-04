#include "ResourceScript.h"
#include "Application.h"
#include "ModuleResources.h"
#include <fstream>

ResourceScript::ResourceScript()
{
	type = ResourceType::RESOURCE_SCRIPT;
}

ResourceScript::~ResourceScript()
{
}

bool ResourceScript::CreateMetaData(const u64& force_id)
{
	bool ret = true;

	if (force_id != 0)
		ID = force_id;
	else
		ID = App->resources->GetRandomID();

	std::ifstream file(path);
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			if (line.find("ALIEN_ENGINE_CLASS_API") != std::string::npos) {
				bool alien = false;
				if (line.find("Alien") != std::string::npos) {
					alien = true;
				}
				std::string class_name = GetDataStructure(line, "ALIEN_ENGINE_CLASS_API");
				data_structures.push_back({ class_name, alien });
			}
			else if (line.find("ALIEN_ENGINE_STRUCT_API") != std::string::npos) {
				std::string class_name = GetDataStructure(line, "ALIEN_ENGINE_STRUCT_API");
				data_structures.push_back({ class_name, false });
			}
		}
		file.close();
	}

	path = std::string(SCRIPTS_FOLDER + name + ".alienScript").data();

	return ret;
}

std::string ResourceScript::GetDataStructure(const std::string& line, const std::string& api)
{
	std::string data_name;
	bool start_cpoying = false;
	bool api_found = false;
	for (uint i = 0; i < line.size(); ++i) {
		if (!api_found) {
			if (line[i] != ' ' && line[i] != '\t') {
				data_name += line[i];
			}
			else {
				if (App->StringCmp(api.data(), data_name.data())) {
					api_found = true;
				}
				data_name.clear();
			}
		}
		else {
			if (!start_cpoying) {
				if (line[i] != ' ' && line[i] != '\t') {
					start_cpoying = true;
					data_name += line[i];
				}
			}
			else {
				if (line[i] != ' ' && line[i] != '\t') {
					data_name += line[i];
				}
				else
					break;
			}
		}
	}
	return data_name;
}
