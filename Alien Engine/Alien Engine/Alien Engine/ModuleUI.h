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
	bool CleanUp();
	void Draw();
	void MainMenuBar();
	void ChangeStyle(const int& style_number);

private:
	void InitPanels();
	void UpdatePanels();
	Panel*& GetPanelByName(const std::string& panel_name);

public:
	void FramerateRegister(float frames, float ms);

private:
	bool show_demo_wndow = false;
	std::vector<Panel*> panels;

};

