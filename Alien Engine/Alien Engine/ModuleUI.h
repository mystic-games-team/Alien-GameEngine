#pragma once
#include "Module.h"
#include "Globals.h"

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "Panel.h"

class Panel;
class PanelConfig;
class PanelAbout;
class PanelConsole;
class PanelCreateObject;
struct ShortCut;

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
	void ReportBug();
	void ChangeStyle(const int& style_number);
	void ChangeEnableDemo();

private:
	void InitPanels();
	void UpdatePanels();
	Panel*& GetPanelByName(const std::string& panel_name);
	void InitShortCuts();

public:
	void FramerateRegister(float frames, float ms);

private:
	bool show_demo_wndow = false;

	// Panels

	std::vector<Panel*> panels;

	PanelAbout* panel_about = nullptr;
	PanelConsole* panel_console = nullptr;
	PanelCreateObject* panel_create_object = nullptr;

	// ShortCuts

	ShortCut* shortcut_demo = nullptr;
	ShortCut* shortcut_report_bug = nullptr;
	ShortCut* shortcut_wireframe = nullptr;
	ShortCut* shortcut_view_mesh = nullptr;

public:
	PanelConfig* panel_config = nullptr;

};

