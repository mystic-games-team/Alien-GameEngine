#ifndef __MODULE_UI__
#define __MODULE_UI__

#include "Globals.h"
#include "Application.h"

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

};

#endif // !__MODULE_UI__
