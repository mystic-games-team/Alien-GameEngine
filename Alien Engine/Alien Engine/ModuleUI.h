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
class PanelRender;
class PanelHierarchy;
class PanelInspector;

struct ShortCut;

class ModuleUI : public Module
{
public:
	ModuleUI(bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status PreUpdate(float dt);
	bool CleanUp();
	void LoadConfig(JSONfilepack*& config);
	void SaveConfig(JSONfilepack*& config);


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
	PanelRender* panel_render = nullptr;
	PanelHierarchy* panel_hierarchy = nullptr;
	PanelInspector* panel_inspector = nullptr;

	// ShortCuts

	ShortCut* shortcut_demo = nullptr;
	ShortCut* shortcut_report_bug = nullptr;
	ShortCut* shortcut_wireframe = nullptr;
	ShortCut* shortcut_view_mesh = nullptr;
	ShortCut* shortcut_view_grid = nullptr;
	ShortCut* shortcut_view_normal_vertex = nullptr;
	ShortCut* shortcut_view_normal_face = nullptr;



	SDL_Scancode panel_config_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode panel_about_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode panel_hierarchy_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode panel_create_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode panel_inspector_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode panel_console_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode panel_render_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode shortcut_demo_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode shortcut_report_bug_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode shortcut_wireframe_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode shortcut_view_mesh_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode shortcut_view_grid_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode shortcut_view_normal_vertex_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };
	SDL_Scancode shortcut_view_normal_face_codes[3] = { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN };


public:
	PanelConfig* panel_config = nullptr;

};

