#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include <gl/GL.h>
#include "PanelAbout.h"
#include "PanelConfig.h"
#include "PanelConsole.h"
#include "PanelCreateObject.h"
#include "PanelHierarchy.h"
#include "PanelRender.h"
#include "SDL/include/SDL_assert.h"
#include "ModuleObjects.h"
#include "PanelInspector.h"

ModuleUI::ModuleUI(bool start_enabled) : Module(start_enabled)
{
	name.assign("UI");
}

ModuleUI::~ModuleUI()
{
}

// Load assets
bool ModuleUI::Start()
{
	LOG("Loading UI assets");
	bool ret = true;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ChangeStyle(App->window->style);


	// Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);

	InitPanels();
	InitShortCuts();
	
	return ret;
}


// Load assets
bool ModuleUI::CleanUp()
{
	LOG("Unloading UI scene");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	std::vector<Panel*>::iterator item = panels.begin();
	for (; item != panels.end(); ++item) {
		if ((*item) != nullptr) {
			delete *item;
			*item = nullptr;
		}
	}
	panels.clear();

	return true;
}

void ModuleUI::LoadConfig(JSONfilepack*& config)
{
	for (uint i = 0; i < 3; ++i) {
		panel_config_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.PanelConfig", i);
		panel_about_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.PanelAbout", i);
		panel_create_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.PanelCreate", i);
		panel_hierarchy_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.PanelHierarchy", i);
		panel_console_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.PanelConsole", i);
		panel_inspector_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.PanelInspector", i);
		panel_render_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.PanelRender", i);
		shortcut_demo_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.ImGuiDemo", i);
		shortcut_report_bug_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.ReportBug", i);
		shortcut_view_mesh_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.ViewMesh", i);
		shortcut_wireframe_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.WireframeMode", i);
		shortcut_view_grid_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.ShowGrid", i);
		shortcut_view_normal_vertex_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.ShowNormalVertex", i);
		shortcut_view_normal_face_codes[i] = (SDL_Scancode)(uint)config->GetArrayNumber("Configuration.UI.ShortCuts.ShowNormalFace", i);
	}

}

void ModuleUI::SaveConfig(JSONfilepack*& config)
{
	for (uint i = 0; i < 3; ++i) {
		config->SetArrayNumber("Configuration.UI.ShortCuts.PanelConfig", (uint)panel_config->shortcut->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.PanelAbout", (uint)panel_about->shortcut->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.PanelHierarchy", (uint)panel_hierarchy->shortcut->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.Render", (uint)panel_render->shortcut->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.PanelInspector", (uint)panel_inspector->shortcut->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.PanelConsole", (uint)panel_console->shortcut->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.PanelCreate", (uint)panel_create_object->shortcut->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.WireframeMode", (uint)shortcut_wireframe->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.ViewMesh", (uint)shortcut_view_mesh->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.ReportBug", (uint)shortcut_report_bug->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.ImGuiDemo", (uint)shortcut_demo->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.ShowGrid", (uint)shortcut_view_grid->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.ShowNormalVertex", (uint)shortcut_view_normal_vertex->GetScancode(i));
		config->SetArrayNumber("Configuration.UI.ShortCuts.ShowNormalFace", (uint)shortcut_view_normal_face->GetScancode(i));
	}
}

update_status ModuleUI::PreUpdate(float dt)
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

void ModuleUI::Draw() {


	if (show_demo_wndow)
		ImGui::ShowDemoWindow(&show_demo_wndow);

	MainMenuBar();
	UpdatePanels();

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN) {
		for (int i = 0;i<10;++i)
			App->objects->CreatePrimitive(PrimitiveType::SPHERE_ALIEN);

	}
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
		App->objects->DeleteAllObjects();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleUI::MainMenuBar()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::BeginMenu("New")) 
		{
				if (ImGui::MenuItem("haha"))
				{
					LOG("haha");
				}
				ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Open")) 
		{

		}
		if (ImGui::MenuItem("Close", "Alt + F4"))
		{
			App->QuitApp();
		}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Configuration", panel_config->shortcut->GetNameScancodes()))
		{
			panel_config->ChangeEnable();
		}
		if (ImGui::MenuItem("Console", panel_console->shortcut->GetNameScancodes()))
		{
			panel_console->ChangeEnable();
		}
		if (ImGui::MenuItem("Inspector", panel_inspector->shortcut->GetNameScancodes()))
		{
			panel_inspector->ChangeEnable();
		}
		if (ImGui::MenuItem("Hierarchy", panel_hierarchy->shortcut->GetNameScancodes()))
		{
			panel_hierarchy->ChangeEnable();
		}
		if (ImGui::MenuItem("Render Options", panel_render->shortcut->GetNameScancodes()))
		{
			panel_render->ChangeEnable();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Create"))
	{
		if (ImGui::MenuItem("Cube"))
		{
			App->objects->CreatePrimitive(PrimitiveType::CUBE);
		}
		if (ImGui::MenuItem("Sphere"))
		{
			App->objects->CreatePrimitive(PrimitiveType::SPHERE_ALIEN);
		}
		if (ImGui::MenuItem("Rock"))
		{
			App->objects->CreatePrimitive(PrimitiveType::ROCK);
		}
		if (ImGui::BeginMenu("Other"))
		{
			if (ImGui::MenuItem("Dodecahedron"))
			{
				App->objects->CreatePrimitive(PrimitiveType::DODECAHEDRON);
			}
			if (ImGui::MenuItem("Octahedron"))
			{
				App->objects->CreatePrimitive(PrimitiveType::OCTAHEDRON);
			}
			if (ImGui::MenuItem("Icosahedron"))
			{
				App->objects->CreatePrimitive(PrimitiveType::ICOSAHEDRON);
			}
			if (ImGui::MenuItem("Torus"))
			{
				App->objects->CreatePrimitive(PrimitiveType::TORUS);
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Create...", panel_create_object->shortcut->GetNameScancodes()))
		{
			panel_create_object->ChangeEnable();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("About", panel_about->shortcut->GetNameScancodes()))
		{
			panel_about->ChangeEnable();
		}
		if (ImGui::MenuItem("Show Gui Demo", shortcut_demo->GetNameScancodes()))
		{
			ChangeEnableDemo();
		}
		if (ImGui::MenuItem("Documentation"))
		{
			LOG("Put link wiki");
		}
		if (ImGui::MenuItem("Report a bug", shortcut_report_bug->GetNameScancodes()))
		{
			ReportBug();
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void ModuleUI::ReportBug()
{
	App->OpenWebsite("https://github.com/VictorSegura99/Alien-GameEngine/issues");
}

void ModuleUI::ChangeStyle(const int& style_number)
{
	switch (style_number) {
	case 0:
		ImGui::StyleColorsClassic();
		break;
	case 1:
		ImGui::StyleColorsDark();
		break;
	case 2:
		ImGui::StyleColorsLight();
		break;
	case 3:
		break;
	}
}

void ModuleUI::ChangeEnableDemo()
{
	show_demo_wndow = !show_demo_wndow;
}

void ModuleUI::InitPanels()
{
	panel_about = new PanelAbout("About Alien Engine", panel_about_codes[0], panel_about_codes[1], panel_about_codes[2]);
	panel_config = new PanelConfig("Configuration", panel_config_codes[0], panel_config_codes[1], panel_config_codes[2]);
	panel_console = new PanelConsole("Console", panel_console_codes[0], panel_console_codes[1], panel_console_codes[2]);
	panel_render = new PanelRender("Render Options", panel_render_codes[0], panel_render_codes[1], panel_render_codes[2]);
	panel_hierarchy = new PanelHierarchy("Panel Hierarchy", panel_hierarchy_codes[0], panel_hierarchy_codes[1], panel_hierarchy_codes[2]);
	panel_create_object = new PanelCreateObject("Create Object", panel_create_codes[0], panel_create_codes[1], panel_create_codes[2]);
	panel_inspector = new PanelInspector("Inspector", panel_inspector_codes[0], panel_inspector_codes[1], panel_inspector_codes[2]);

	panels.push_back(panel_about);
	panels.push_back(panel_config);
	panels.push_back(panel_console);
	panels.push_back(panel_render);
	panels.push_back(panel_hierarchy);
	panels.push_back(panel_create_object);
	panels.push_back(panel_inspector);

}

void ModuleUI::UpdatePanels()
{
	std::vector<Panel*>::iterator item = panels.begin();
	for (; item != panels.end(); ++item) {
		if (*item != nullptr) {
			if ((*item)->IsEnabled()) {
				(*item)->PanelLogic();
			}
		}
	}
}

Panel*& ModuleUI::GetPanelByName(const std::string& panel_name)
{
	std::vector<Panel*>::iterator item = panels.begin();
	for (; item != panels.end(); ++item) {
		if (*item != nullptr && (*item)->GetName() == panel_name) {
			return (*item);
		}
	}
	SDL_assert(1 == 0); //panel name is not correct, revise panels names!!
}
                  
void ModuleUI::InitShortCuts()
{
	shortcut_demo = App->shortcut_manager->AddShortCut("imGui Demo", shortcut_demo_codes[0], std::bind(&ModuleUI::ChangeEnableDemo, App->ui), shortcut_demo_codes[1], shortcut_demo_codes[2]);
	shortcut_report_bug = App->shortcut_manager->AddShortCut("Report Bug", shortcut_report_bug_codes[0], std::bind(&ModuleUI::ReportBug, App->ui), shortcut_report_bug_codes[1], shortcut_report_bug_codes[2]);
	shortcut_view_mesh = App->shortcut_manager->AddShortCut("Mesh View", shortcut_view_mesh_codes[0], std::bind(&ModuleObjects::ChangeViewMeshMode, App->objects), shortcut_view_mesh_codes[1], shortcut_view_mesh_codes[2]);
	shortcut_wireframe = App->shortcut_manager->AddShortCut("Wireframe Mode", shortcut_wireframe_codes[0], std::bind(&ModuleObjects::ChangeWireframeMode, App->objects), shortcut_wireframe_codes[1], shortcut_wireframe_codes[2]);
	shortcut_view_grid = App->shortcut_manager->AddShortCut("View Grid", shortcut_view_grid_codes[0], std::bind(&ModuleObjects::ChangeEnableGrid, App->objects), shortcut_view_grid_codes[1], shortcut_view_grid_codes[2]);
	shortcut_view_normal_vertex = App->shortcut_manager->AddShortCut("View Normal Vertex", shortcut_view_normal_vertex_codes[0], std::bind(&ModuleObjects::ChangeEnableNormalVertex, App->objects), shortcut_view_normal_vertex_codes[1], shortcut_view_normal_vertex_codes[2]);
	shortcut_view_normal_face = App->shortcut_manager->AddShortCut("View Normal Face", shortcut_view_normal_face_codes[0], std::bind(&ModuleObjects::ChangeEnableNormalFace, App->objects), shortcut_view_normal_face_codes[1], shortcut_view_normal_face_codes[2]);

	// OrderShortCuts must be called after all shortcuts have been created!! Victor read this...
	App->shortcut_manager->OrderShortCuts();
}

void ModuleUI::FramerateRegister(float frames, float ms)
{
	if (panel_config->IsEnabled())
		panel_config->FramerateInfo(frames,ms);
}



