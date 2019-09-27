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

	switch (App->window->style) {
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


	// Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);

	InitPanels();

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

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::BeginMenu("New", "Ctrl+N")) 
		{
				if (ImGui::MenuItem("haha"))
				{
					LOG("haha");
				}
				ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Open", "Ctrl+W")) 
		{

		}
		if (ImGui::MenuItem("Close", "Ctrl+W")) 
		{
			App->QuitApp();
		}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Configuration", "Ctrl + O"))
		{
			GetPanelByName("Configuration")->ChangeEnable();
		}
		if (ImGui::MenuItem("Console", "Ctrl + T"))
		{
			GetPanelByName("Console")->ChangeEnable();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("About", "Ctrl + A"))
		{
			GetPanelByName("About Alien Engine")->ChangeEnable();
		}
		if (ImGui::MenuItem("Show Gui Demo"))
		{
			show_demo_wndow = !show_demo_wndow;
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

void ModuleUI::InitPanels()
{
	panels.push_back(new PanelAbout("About Alien Engine", std::vector<SDL_Scancode>{SDL_SCANCODE_LCTRL, SDL_SCANCODE_A}));
	panels.push_back(new PanelConfig("Configuration", std::vector<SDL_Scancode>{SDL_SCANCODE_LCTRL, SDL_SCANCODE_O}));
	panels.push_back(new PanelConsole("Console", std::vector<SDL_Scancode>{SDL_SCANCODE_LCTRL, SDL_SCANCODE_T}));

}

void ModuleUI::UpdatePanels()
{
	std::vector<Panel*>::iterator item = panels.begin();
	for (; item != panels.end(); ++item) {
		if (*item != nullptr) {
			if ((*item)->ShortCutClicked())
				(*item)->ChangeEnable();
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
	PanelConfig* pc = (PanelConfig*)GetPanelByName("Configuration");
	if (pc->IsEnabled())
		pc->FramerateInfo(frames,ms);
}



