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
#include "SDL/include/SDL_assert.h"

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

	shortcut_demo = App->shortcut_manager->AddShortCut(SDL_SCANCODE_D, std::bind(&ModuleUI::ChangeEnableDemo, App->ui), SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL);

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


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleUI::MainMenuBar()
{
	static char shortcut_char[50];
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
		if (ImGui::MenuItem("Close")) 
		{
			App->QuitApp();
		}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View"))
	{
		sprintf_s(shortcut_char, 50, "%s / %s + %s", SDL_GetScancodeName(panel_config->shortcut->key2_repeat), SDL_GetScancodeName(panel_config->shortcut->key3_repeat_extra), SDL_GetScancodeName(panel_config->shortcut->key1_down));
		if (ImGui::MenuItem("Configuration", shortcut_char))
		{
			panel_config->ChangeEnable();
		}
		sprintf_s(shortcut_char, 50, "%s / %s + %s", SDL_GetScancodeName(panel_console->shortcut->key2_repeat), SDL_GetScancodeName(panel_console->shortcut->key3_repeat_extra), SDL_GetScancodeName(panel_console->shortcut->key1_down));
		if (ImGui::MenuItem("Console", shortcut_char))
		{
			panel_console->ChangeEnable();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		sprintf_s(shortcut_char, 50, "%s / %s + %s", SDL_GetScancodeName(panel_about->shortcut->key2_repeat), SDL_GetScancodeName(panel_about->shortcut->key3_repeat_extra), SDL_GetScancodeName(panel_about->shortcut->key1_down));
		if (ImGui::MenuItem("About", shortcut_char))
		{
			panel_about->ChangeEnable();
		}
		sprintf_s(shortcut_char, 50, "%s / %s + %s", SDL_GetScancodeName(shortcut_demo->key2_repeat), SDL_GetScancodeName(shortcut_demo->key3_repeat_extra), SDL_GetScancodeName(shortcut_demo->key1_down));
		if (ImGui::MenuItem("Show Gui Demo", shortcut_char))
		{
			ChangeEnableDemo();
		}
		if (ImGui::MenuItem("Documentation"))
		{
			LOG("Put link wiki");
		}
		if (ImGui::MenuItem("Report a bug"))
		{
			App->OpenWebsite("https://github.com/VictorSegura99/Alien-GameEngine/issues");
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
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
	panel_about = new PanelAbout("About Alien Engine", SDL_SCANCODE_A, SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL);
	panel_config = new PanelConfig("Configuration", SDL_SCANCODE_O, SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL);
	panel_console = new PanelConsole("Console", SDL_SCANCODE_T, SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL);

	panels.push_back(panel_about);
	panels.push_back(panel_config);
	panels.push_back(panel_console);

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

void ModuleUI::FramerateRegister(float frames, float ms)
{
	if (panel_config->IsEnabled())
		panel_config->FramerateInfo(frames,ms);
}



