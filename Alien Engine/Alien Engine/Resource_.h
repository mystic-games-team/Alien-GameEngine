#pragma once

#include <string>

typedef unsigned int uint;

class Resource {

public:

	Resource();
	virtual ~Resource();

	// path
	const char* const GetPath() const;

	// name
	const char* const GetName() const;

protected:

	std::string path;
	std::string name;

};