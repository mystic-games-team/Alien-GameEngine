#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include <gl/GL.h>
#include "PanelAbout.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls ImGuiWindowFlags_MenuBar

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	//ImGui_ImplGlfw_InitForOpenGL(window, true);
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

// Update
update_status ModuleUI::Update(float dt)
{
	bool show_demo_wndow = true;
	ImGui::ShowDemoWindow(&show_demo_wndow);

	MainMenuBar();
	UpdatePanels();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float dt)
{

	ImGui::Render();


	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	return UPDATE_CONTINUE;
}



void ModuleUI::MainMenuBar()
{
	/*ImGui::Begin("TestMenu", (bool*)0, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Close", "Ctrl+W")) { 
				App->QuitApp();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();*/

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

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("About Us..."))
		{
			App->OpenWebsite("https://github.com/VictorSegura99/Alien-GameEngine");
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void ModuleUI::InitPanels()
{
	panels.push_back(new PanelAbout("About", (std::vector<SDL_Scancode>)(SDL_SCANCODE_LCTRL, SDL_SCANCODE_A)));


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



