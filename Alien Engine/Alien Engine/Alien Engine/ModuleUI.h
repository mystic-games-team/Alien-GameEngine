#pragma once
#include "Module.h"
#include "Globals.h"

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "Panel.h"

class Panel;


class ModuleUI : public Module
{
public:
	ModuleUI(bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void MainMenuBar();

private:
	void InitPanels();
	void UpdatePanels();
	Panel*& GetPanelByName(const std::string& panel_name);

public:

private:
	bool show_demo_wndow = false;
	std::vector<Panel*> panels;

};

