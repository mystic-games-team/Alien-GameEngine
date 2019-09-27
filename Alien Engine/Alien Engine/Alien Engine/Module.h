#pragma once

#include <string>
#include "JSONfilepack.h"

class Module
{
private :
	bool enabled = true;

public:

	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual void LoadConfig(JSONfilepack*& config) {}
	virtual void SaveConfig(JSONfilepack*& config) {}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}
		
	std::string name;

};