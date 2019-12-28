#pragma once

#include "Resource_.h"

#include <utility>
#include <vector>
#include <string>

class ResourceScript : public Resource {
public:
	ResourceScript();
	virtual ~ResourceScript();

	bool CreateMetaData(const u64& force_id = 0);
	bool ReadBaseInfo(const char* assets_file_path);
	void ReadLibrary(const char* meta_data);
	bool NeedReload() const;
private:

	std::string GetDataStructure(const std::string& line, const std::string& api);

private:

	time_t last_time_mod = 0;

public:
	bool reload_completed = false;
	std::vector<std::pair<std::string, bool>> data_structures;

	std::string header_path;
};
