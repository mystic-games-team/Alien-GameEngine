#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <gl/GL.h>

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
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();
	


	return ret;
}


// Load assets
bool ModuleUI::CleanUp()
{
	LOG("Unloading UI scene");


	return true;
}

update_status ModuleUI::PreUpdate(float dt)
{

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

// Update
update_status ModuleUI::Update(float dt)
{
	bool show_demo_wndow = true;
	ImGui::ShowDemoWindow(&show_demo_wndow);
	Menu();
	Close_Menu();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float dt)
{

	ImGui::Render();

	ImVec4 clear_color = ImVec4(0.1f, 0.87f, 0.32f, 0.64f);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);

	// If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!), 
	// you may need to backup/reset/restore current shader using the commented lines below.
	//GLint last_program; 
	//glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	//glUseProgram(0);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	//glUseProgram(last_program);


	return UPDATE_CONTINUE;
}



void ModuleUI::Menu()
{
	ImGui::Begin("TestMenu", (bool*)0, ImGuiWindowFlags_MenuBar);
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
	ImGui::End();
}

void ModuleUI::Close_Menu()
{
	ImGui::Begin("PANIC BUTTON", (bool*)0, ImGuiWindowFlags_MenuBar| ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoTitleBar);
	
	if (ImGui::Button("PANIC CLOSE",ImVec2(250, 250)))
	{
		App->QuitApp();
	}
	ImGui::End();
}



