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
#include "PanelLayout.h"
#include "PanelInspector.h"
#include "PanelProject.h"
#include "PanelSceneSelector.h"
#include "PanelScene.h"
#include "PanelGame.h"
#include <string>
#include "ResourceTexture.h"
#include "ReturnZ.h"

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
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.IniFilename = NULL;
	io.WantSaveIniSettings = false;

	ChangeStyle(App->window->style);


	// Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);

	InitPanels();
	InitShortCuts();

	LoadLayouts();
	LoadActiveLayout();

	return ret;
}


// Load assets
bool ModuleUI::CleanUp()
{
	LOG("Unloading UI scene");

	if (!need_to_save_layouts) // just save to know which is active
		SaveLayoutsActive();
	else
		SaveAllLayouts();

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

	std::vector<Layout*>::iterator it = layouts.begin();
	for (; it != layouts.end(); ++it) {
		if ((*it) != nullptr) {
			delete* it;
			*it = nullptr;
		}
	}
	layouts.clear();

	return true;
}

void ModuleUI::LoadConfig(JSONfilepack*& config)
{
	uint size_of_codes = sizeof(panel_config_codes);
	memcpy(panel_config_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.PanelConfig"), size_of_codes);
	memcpy(panel_about_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.PanelAbout"), size_of_codes);
	memcpy(panel_create_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.PanelCreate"), size_of_codes);
	memcpy(panel_project_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.PanelProject"), size_of_codes);
	memcpy(panel_hierarchy_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.PanelHierarchy"), size_of_codes);
	memcpy(panel_console_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.PanelConsole"), size_of_codes);
	memcpy(panel_scene_selector_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.PanelSceneSelector"), size_of_codes);
	memcpy(panel_inspector_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.PanelInspector"), size_of_codes);
	memcpy(panel_layout_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.PanelLayout"), size_of_codes);
	memcpy(panel_render_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.PanelRender"), size_of_codes);
	memcpy(panel_scene_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.PanelScene"), size_of_codes);
	memcpy(shortcut_demo_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.ImGuiDemo"), size_of_codes);
	memcpy(shortcut_report_bug_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.ReportBug"), size_of_codes);
	memcpy(shortcut_view_mesh_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.ViewMesh"), size_of_codes);
	memcpy(shortcut_wireframe_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.WireframeMode"), size_of_codes);
	memcpy(shortcut_view_grid_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.ShowGrid"), size_of_codes);
	memcpy(shortcut_view_normal_vertex_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.ShowNormalVertex"), size_of_codes);
	memcpy(shortcut_view_normal_face_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.ShowNormalFace"), size_of_codes);
	memcpy(shortcut_save_scene_as_new_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.SaveSceneAsNew"), size_of_codes);
	memcpy(shortcut_load_scene_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.LoadScene"), size_of_codes);
	memcpy(shortcut_new_scene_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.NewScene"), size_of_codes);
	memcpy(shortcut_outline_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.EnableOutline"), size_of_codes);
	memcpy(shortcut_AABB_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.AABB"), size_of_codes);
	memcpy(shortcut_OBB_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.OBB"), size_of_codes);
	memcpy(shortcut_octree_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.ViewOctree"), size_of_codes);
	memcpy(panel_game_codes, config->GetShortcutCodes("Configuration.UI.ShortCuts.Game"), size_of_codes);


	if (panel_about != nullptr) {
		panel_about->shortcut->SetShortcutKeys(panel_about_codes[0], panel_about_codes[1], panel_about_codes[2]);
		panel_config->shortcut->SetShortcutKeys(panel_config_codes[0], panel_config_codes[1], panel_config_codes[2]);
		panel_scene_selector->shortcut->SetShortcutKeys(panel_scene_selector_codes[0], panel_scene_selector_codes[1], panel_scene_selector_codes[2]);
		panel_project->shortcut->SetShortcutKeys(panel_project_codes[0], panel_project_codes[1], panel_project_codes[2]);
		panel_console->shortcut->SetShortcutKeys(panel_console_codes[0], panel_console_codes[1], panel_console_codes[2]);
		panel_render->shortcut->SetShortcutKeys(panel_render_codes[0], panel_render_codes[1], panel_render_codes[2]);
		panel_hierarchy->shortcut->SetShortcutKeys(panel_hierarchy_codes[0], panel_hierarchy_codes[1], panel_hierarchy_codes[2]);
		panel_create_object->shortcut->SetShortcutKeys(panel_create_codes[0], panel_create_codes[1], panel_create_codes[2]);
		panel_inspector->shortcut->SetShortcutKeys(panel_inspector_codes[0], panel_inspector_codes[1], panel_inspector_codes[2]);
		panel_scene->shortcut->SetShortcutKeys(panel_scene_codes[0], panel_scene_codes[1], panel_scene_codes[2]);
		panel_game->shortcut->SetShortcutKeys(panel_game_codes[0], panel_game_codes[1], panel_game_codes[2]);
		panel_layout->shortcut->SetShortcutKeys(panel_layout_codes[0], panel_layout_codes[1], panel_layout_codes[2]);
		shortcut_demo->SetShortcutKeys(shortcut_demo_codes[0], shortcut_demo_codes[1], shortcut_demo_codes[2]);
		shortcut_report_bug->SetShortcutKeys(shortcut_report_bug_codes[0], shortcut_report_bug_codes[1], shortcut_report_bug_codes[2]);
		shortcut_view_mesh->SetShortcutKeys(shortcut_view_mesh_codes[0], shortcut_view_mesh_codes[1], shortcut_view_mesh_codes[2]);
		shortcut_wireframe->SetShortcutKeys(shortcut_wireframe_codes[0], shortcut_wireframe_codes[1], shortcut_wireframe_codes[2]);
		shortcut_view_grid->SetShortcutKeys(shortcut_view_grid_codes[0], shortcut_view_grid_codes[1], shortcut_view_grid_codes[2]);
		shortcut_view_normal_vertex->SetShortcutKeys(shortcut_view_normal_vertex_codes[0], shortcut_view_normal_vertex_codes[1], shortcut_view_normal_vertex_codes[2]);
		shortcut_view_normal_face->SetShortcutKeys(shortcut_view_normal_face_codes[0], shortcut_view_normal_face_codes[1], shortcut_view_normal_face_codes[2]);
		shortcut_load_scene->SetShortcutKeys(shortcut_load_scene_codes[0], shortcut_load_scene_codes[1], shortcut_load_scene_codes[2]);
		shortcut_save_scene_as_new->SetShortcutKeys(shortcut_save_scene_as_new_codes[0], shortcut_save_scene_as_new_codes[1], shortcut_save_scene_as_new_codes[2]);
		shortcut_new_scene->SetShortcutKeys(shortcut_new_scene_codes[0], shortcut_new_scene_codes[1], shortcut_new_scene_codes[2]);
		shortcut_outline->SetShortcutKeys(shortcut_outline_codes[0], shortcut_outline_codes[1], shortcut_outline_codes[2]);
		shortcut_AABB->SetShortcutKeys(shortcut_AABB_codes[0], shortcut_AABB_codes[1], shortcut_AABB_codes[2]);
		shortcut_OBB->SetShortcutKeys(shortcut_OBB_codes[0], shortcut_OBB_codes[1], shortcut_OBB_codes[2]);
		shortcut_octree->SetShortcutKeys(shortcut_octree_codes[0], shortcut_octree_codes[1], shortcut_octree_codes[2]);

		// OrderShortCuts must be called after all shortcuts have been created!! Victor read this...
		App->shortcut_manager->OrderShortCuts();
	}
}

void ModuleUI::SaveConfig(JSONfilepack*& config)
{
	config->SetShortcutCodes("Configuration.UI.ShortCuts.PanelConfig", panel_config->shortcut->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.PanelProject", panel_project->shortcut->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.PanelAbout", panel_about->shortcut->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.PanelHierarchy", panel_hierarchy->shortcut->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.PanelRender", panel_render->shortcut->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.PanelInspector", panel_inspector->shortcut->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.PanelConsole", panel_console->shortcut->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.PanelCreate", panel_create_object->shortcut->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.PanelScene", panel_scene->shortcut->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.PanelSceneSelector", panel_scene_selector->shortcut->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.PanelLayout", panel_layout->shortcut->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.WireframeMode", shortcut_wireframe->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.ViewMesh", shortcut_view_mesh->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.ReportBug", shortcut_report_bug->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.ImGuiDemo", shortcut_demo->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.ShowGrid", shortcut_view_grid->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.ShowNormalVertex", shortcut_view_normal_vertex->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.ShowNormalFace", shortcut_view_normal_face->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.LoadScene", shortcut_load_scene->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.SaveSceneAsNew", shortcut_save_scene_as_new->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.NewScene", shortcut_new_scene->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.EnableOutline", shortcut_outline->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.AABB", shortcut_AABB->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.OBB", shortcut_OBB->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.ViewOctree", shortcut_octree->GetScancodesArray());
	config->SetShortcutCodes("Configuration.UI.ShortCuts.Game", shortcut_game->GetScancodesArray());
}
void ModuleUI::LoadLayouts()
{
	JSONfilepack* json_layout = App->GetJSONLayout();
	
	number_of_layouts = json_layout->GetNumber("Layouts.Number");

	for (uint i = 1; i <= number_of_layouts; ++i) {
		Layout* layout = new Layout();
		layout->number = i;
		std::string json_path("Layouts.Layout" + std::to_string(i));
		layout->name = json_layout->GetString(json_path + std::string(".Name"));
		layout->path = json_layout->GetString(json_path + std::string(".Path"));
		layout->active = json_layout->GetBoolean(json_path + std::string(".Active"));

		if (layout->active && active_layout == nullptr)
			active_layout = layout;
		else if (layout->active)
			layout->active = false;

		std::vector<Panel*>::iterator panel = panels.begin();
		for (; panel != panels.end(); ++panel) {
			if (*panel != nullptr) {
				layout->panels_enabled.push_back(json_layout->GetBoolean(json_path + std::string(".") + (*panel)->GetPanelName()));
			}
		}

		layouts.push_back(layout);
	}
}

void ModuleUI::SaveAllLayouts()
{
	JSONfilepack* json_layout = App->GetJSONLayout();
	App->DeleteJSONfile(json_layout); 
	App->file_system->Remove("Configuration/LayoutsInfo.json");
	json_layout = App->CreateJSONFile("Configuration/LayoutsInfo.json");
	
	json_layout->StartSave();

	json_layout->SetNumber("Layouts.Number", number_of_layouts);

	std::vector<Layout*>::iterator item = layouts.begin();
	for (; item != layouts.end(); ++item) {
		if (*item != nullptr) {
			std::string json_path("Layouts.Layout" + std::to_string((item - layouts.begin()) + 1));
			json_layout->SetNumber(json_path + std::string(".Number"), (item - layouts.begin()) + 1);
			json_layout->SetBoolean(json_path + std::string(".Active"), (*item)->active);
			json_layout->SetString(json_path + std::string(".Name"), (*item)->name);
			json_layout->SetString(json_path + std::string(".Path"), (*item)->path);

			std::vector<Panel*>::iterator panel = panels.begin();
			for (; panel != panels.end(); ++panel) {
				if (*panel != nullptr) {
					bool enabled = (*panel)->IsEnabled();
					json_layout->SetBoolean(json_path + std::string(".") + (*panel)->GetPanelName(), enabled);
				}
			}

		}
	}

	json_layout->FinishSave();
}

void ModuleUI::SaveLayout(Layout* layout, bool is_new)
{
	JSONfilepack* json_layout = App->GetJSONLayout();
	json_layout->StartSave();

	std::string json_path;
	if (is_new) {
		json_layout->SetNumber("Layouts.Number", number_of_layouts);
		json_path = ("Layouts.Layout" + std::to_string(number_of_layouts));
	}
	else
		json_path = ("Layouts.Layout" + std::to_string(layout->number));

	json_layout->SetNumber(json_path + std::string(".Number"), layout->number);
	json_layout->SetBoolean(json_path + std::string(".Active"), layout->active);
	json_layout->SetString(json_path + std::string(".Name"), layout->name);
	json_layout->SetString(json_path + std::string(".Path"), layout->path);
	layout->panels_enabled.clear();
	std::vector<Panel*>::iterator panel = panels.begin();
	for (; panel != panels.end(); ++panel) {
		if (*panel != nullptr) {
			bool enabled = (*panel)->IsEnabled();
			json_layout->SetBoolean(json_path + std::string(".") + (*panel)->GetPanelName(), enabled);
			layout->panels_enabled.push_back(enabled);
		}
	}

	json_layout->FinishSave();

	if (!is_new)
		ImGui::SaveIniSettingsToDisk(layout->path.data());
}

void ModuleUI::SaveLayoutsActive()
{
	// just save the active attribut & name to know in the next session which layout was active

	JSONfilepack* json_layout = App->GetJSONLayout();
	json_layout->StartSave();

	std::vector<Layout*>::iterator item = layouts.begin();
	for (; item != layouts.end(); ++item) {
		if (*item != nullptr) {
			std::string json_path("Layouts.Layout" + std::to_string((item - layouts.begin()) + 1));
			json_layout->SetBoolean(json_path + std::string(".Active"), (*item)->active);
			json_layout->SetString(json_path + std::string(".Name"), (*item)->name.data());
		}
	}

	json_layout->FinishSave();
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
	BackgroundDockspace();
	SecondMenuBar();
	UpdatePanels();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleUI::MainMenuBar()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Create New Scene", shortcut_new_scene->GetNameScancodes()))
		{
			panel_scene_selector->OrganizeSave(PanelSceneSelector::SceneSelectorState::CREATE_NEW_SCENE);
		}
		if (ImGui::MenuItem("Save Scene", panel_scene_selector->shortcut->GetNameScancodes()))
		{
			panel_scene_selector->OrganizeSave(PanelSceneSelector::SceneSelectorState::SAVE_SCENE);
		}	
		if (ImGui::MenuItem("Save Scene As", shortcut_save_scene_as_new->GetNameScancodes()))
		{
			panel_scene_selector->OrganizeSave(PanelSceneSelector::SceneSelectorState::SAVE_AS_NEW);
		}
		if (ImGui::MenuItem("Load Scene", shortcut_load_scene->GetNameScancodes()))
		{
			panel_scene_selector->OrganizeSave(PanelSceneSelector::SceneSelectorState::LOAD_SCENE);
		}
		if (ImGui::MenuItem("Import"))
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
		if (ImGui::MenuItem("Scene", panel_scene->shortcut->GetNameScancodes()))
		{
			panel_scene->ChangeEnable();
		}
		if (ImGui::MenuItem("Inspector", panel_inspector->shortcut->GetNameScancodes()))
		{
			panel_inspector->ChangeEnable();
		}
		if (ImGui::MenuItem("Hierarchy", panel_hierarchy->shortcut->GetNameScancodes()))
		{
			panel_hierarchy->ChangeEnable();
		}
		if (ImGui::MenuItem("Panel Project", panel_project->shortcut->GetNameScancodes()))
		{
			panel_project->ChangeEnable();
		}
		if (ImGui::MenuItem("Render Options", panel_render->shortcut->GetNameScancodes()))
		{
			panel_render->ChangeEnable();
		}
		if (ImGui::MenuItem("Game", panel_render->shortcut->GetNameScancodes()))
		{
			panel_game->ChangeEnable();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Create"))
	{
		if (ImGui::MenuItem("Crete Empty GameObject"))
		{
			App->objects->CreateEmptyGameObject(nullptr);
		}
		if (ImGui::MenuItem("Cube"))
		{
			App->objects->CreateBasePrimitive(PrimitiveType::CUBE);
		}
		if (ImGui::MenuItem("Sphere"))
		{
			App->objects->CreateBasePrimitive(PrimitiveType::SPHERE_ALIEN);
		}
		if (ImGui::MenuItem("Rock"))
		{
			App->objects->CreateBasePrimitive(PrimitiveType::ROCK);
		}
		if (ImGui::MenuItem("Torus"))
		{
			App->objects->CreateBasePrimitive(PrimitiveType::TORUS);
		}
		if (ImGui::BeginMenu("Other"))
		{
			if (ImGui::MenuItem("Dodecahedron"))
			{
				App->objects->CreateBasePrimitive(PrimitiveType::DODECAHEDRON);
			}
			if (ImGui::MenuItem("Octahedron"))
			{
				App->objects->CreateBasePrimitive(PrimitiveType::OCTAHEDRON);
			}
			if (ImGui::MenuItem("Icosahedron"))
			{
				App->objects->CreateBasePrimitive(PrimitiveType::ICOSAHEDRON);
			}
			if (ImGui::MenuItem("Klein Bottle"))
			{
				App->objects->CreateBasePrimitive(PrimitiveType::KLEIN_BOTTLE);
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Create...", panel_create_object->shortcut->GetNameScancodes()))
		{
			panel_create_object->ChangeEnable();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Layout")) {
		if (ImGui::MenuItem("Save Actual Layout", active_layout->name.data())) {
			SaveLayout(active_layout, false);
		}
		if (ImGui::MenuItem("Edit Layouts", panel_layout->shortcut->GetNameScancodes())) {
			panel_layout->ChangeEnable();
			static_cast<PanelLayout*>(panel_layout)->is_editor_panel = true;
		}
		if (ImGui::MenuItem("Save Actual Distribution", "Create New Layout")) {
			panel_layout->ChangeEnable();
			static_cast<PanelLayout*>(panel_layout)->is_editor_panel = false;
		}
		if (ImGui::BeginMenu("Set Layout"))
		{
			std::vector<Layout*>::iterator item = layouts.begin();
			for (; item != layouts.end(); ++item) {
				if (*item != nullptr) {
					if (ImGui::MenuItem((*item)->name.data())) {
						ResetImGui();
						active_layout->active = false;
						active_layout = *item;
						(*item)->active = true;
						LoadActiveLayout();
						ImGui::NewFrame();
						return;
					}
				}
			}
			ImGui::EndMenu();
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

void ModuleUI::SecondMenuBar()
{
	ImGui::Begin("## Camera options", (bool*)false, ImGuiWindowFlags_NoDecoration);

	// TODO: Update Control if we use shortcuts
	ImGui::SetCursorPosY((ImGui::GetWindowHeight() * 0.5f) - 15);
	ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	if (ImGui::ImageButton((ImTextureID)App->resources->icons.undo->id, ImVec2(30, 30)))
	{
		ReturnZ::GoBackOneAction();
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Undo (Control+Z)");
		ImGui::EndTooltip();
	}
	ImGui::SameLine();

	if (ImGui::ImageButton((ImTextureID)App->resources->icons.redo->id, ImVec2(30, 30)))
	{
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Redo (Control+Y)");
		ImGui::EndTooltip();
	}
	// Vertical Separator

	ImGui::SameLine();
	ImGui::SetCursorPosY((ImGui::GetWindowHeight() * 0.5f) - 5);
	ImGui::Text("|");
	ImGui::SameLine();

	// Transform Buttons
	ImGui::SetCursorPosY((ImGui::GetWindowHeight() * 0.5f) - 15);
	if (ImGui::ImageButton((ImTextureID)App->resources->icons.move_transform->id, ImVec2(30, 30)))
	{
		panel_scene->guizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Move (W)");
		ImGui::EndTooltip();
	}
	ImGui::SameLine();

	if (ImGui::ImageButton((ImTextureID)App->resources->icons.rotate_transform->id, ImVec2(30, 30)))
	{
		panel_scene->guizmo_operation = ImGuizmo::OPERATION::ROTATE;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Rotate (E)");
		ImGui::EndTooltip();
	}
	ImGui::SameLine();

	if (ImGui::ImageButton((ImTextureID)App->resources->icons.scale_transform->id, ImVec2(30, 30)))
	{
		panel_scene->guizmo_operation = ImGuizmo::OPERATION::SCALE;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Scale (R)");
		ImGui::EndTooltip();
	}

	// Vertical Separator

	ImGui::SameLine();
	ImGui::SetCursorPosY((ImGui::GetWindowHeight() * 0.5f) - 5);
	ImGui::Text("|");
	ImGui::SameLine();

	// Transform Modes
	ImGui::SetCursorPosY((ImGui::GetWindowHeight() * 0.5f) - 15);
	if (ImGui::ImageButton((ImTextureID)App->resources->icons.global->id, ImVec2(30, 30)))
	{
		panel_scene->guizmo_mode = ImGuizmo::MODE::WORLD;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("World Mode (Shift+W)");
		ImGui::EndTooltip();
	}

	ImGui::SameLine();

	if (ImGui::ImageButton((ImTextureID)App->resources->icons.local->id, ImVec2(30, 30)))
	{
		panel_scene->guizmo_mode = ImGuizmo::MODE::LOCAL;
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Local Mode (Shift+L)");
		ImGui::EndTooltip();
	}
	ImGui::PopStyleColor();

	// Vertical Separator

	ImGui::SameLine();
	ImGui::SetCursorPosY((ImGui::GetWindowHeight() * 0.5f) - 5);
	ImGui::Text("|");
	ImGui::SameLine();

	// Camera Combo
	ImGui::SetCursorPosY((ImGui::GetWindowHeight() * 0.5f) - 7);

	static int camera_combo = 0;
	std::string combo_cameras_name;

	if (App->objects->game_cameras.empty())
	{
		actual_name = "No selected camera";
	}
	else
	{
		actual_name = App->renderer3D->actual_game_camera->game_object_attached->GetName();
	}

	ImGui::PushItemWidth(175);
	if (ImGui::BeginCombo("Current Game Camera", actual_name))
	{
		for (std::vector<ComponentCamera*>::iterator iter = App->objects->game_cameras.begin(); iter != App->objects->game_cameras.end(); ++iter)
		{
			bool is_selected = (actual_name == (*iter)->game_object_attached->GetName());
			if (ImGui::Selectable((*iter)->game_object_attached->GetName(), is_selected))
			{
				actual_name = (*iter)->game_object_attached->GetName();
			}
			if (ImGui::IsItemClicked())
			{
				ImGui::SetItemDefaultFocus();
				App->renderer3D->actual_game_camera = (*iter);
			}
		}
		ImGui::EndCombo();
	}

	ImGui::End();
}

void ModuleUI::ResetImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	//
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.IniFilename = NULL;
	io.WantSaveIniSettings = false;

	ChangeStyle(App->window->style);

	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
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

void ModuleUI::DeleteLayout(Layout* layout)
{

	if (layout == active_layout) {
		layout->active = false;
		active_layout = layouts.front();
		active_layout->active = true;
	}
	std::vector<Layout*>::iterator item = layouts.begin();
	while (item != layouts.end()) {
		if (*item != nullptr && *item == layout) {
			App->file_system->Remove((*item)->path.data());
			need_to_save_layouts = true;
			number_of_layouts--;
			delete* item;
			*item = nullptr;
			layouts.erase(item);
			break;
		}
		++item;
	}
}

void ModuleUI::InitPanels()
{
	panel_about = new PanelAbout("About Alien Engine", panel_about_codes[0], panel_about_codes[1], panel_about_codes[2]);
	panel_project = new PanelProject("Panel Project", panel_project_codes[0], panel_project_codes[1], panel_project_codes[2]);
	panel_config = new PanelConfig("Configuration", panel_config_codes[0], panel_config_codes[1], panel_config_codes[2]);
	panel_console = new PanelConsole("Console", panel_console_codes[0], panel_console_codes[1], panel_console_codes[2]);
	panel_render = new PanelRender("Render Options", panel_render_codes[0], panel_render_codes[1], panel_render_codes[2]);
	panel_hierarchy = new PanelHierarchy("Panel Hierarchy", panel_hierarchy_codes[0], panel_hierarchy_codes[1], panel_hierarchy_codes[2]);
	panel_create_object = new PanelCreateObject("Create Object", panel_create_codes[0], panel_create_codes[1], panel_create_codes[2]);
	panel_inspector = new PanelInspector("Inspector", panel_inspector_codes[0], panel_inspector_codes[1], panel_inspector_codes[2]);
	panel_scene = new PanelScene("Scene", panel_scene_codes[0], panel_scene_codes[1], panel_scene_codes[2]);
	panel_scene_selector = new PanelSceneSelector("Save", panel_scene_selector_codes[0], panel_scene_selector_codes[1], panel_scene_selector_codes[2]);
	panel_layout = new PanelLayout("Layout Editor", panel_layout_codes[0], panel_layout_codes[1], panel_layout_codes[2]);
	panel_game = new PanelGame("Game", panel_game_codes[0], panel_game_codes[1], panel_game_codes[2]);

	panels.push_back(panel_about);
	panels.push_back(panel_config);
	panels.push_back(panel_console);
	panels.push_back(panel_project);
	panels.push_back(panel_render);
	panels.push_back(panel_hierarchy);
	panels.push_back(panel_create_object);
	panels.push_back(panel_inspector);
	panels.push_back(panel_game);
	panels.push_back(panel_scene);
	panels.push_back(panel_layout);
	panels.push_back(panel_scene_selector);
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
	shortcut_view_normal_vertex = App->shortcut_manager->AddShortCut("Normal Vertex", shortcut_view_normal_vertex_codes[0], std::bind(&ModuleObjects::ChangeEnableNormalVertex, App->objects), shortcut_view_normal_vertex_codes[1], shortcut_view_normal_vertex_codes[2]);
	shortcut_view_normal_face = App->shortcut_manager->AddShortCut("Normal Face", shortcut_view_normal_face_codes[0], std::bind(&ModuleObjects::ChangeEnableNormalFace, App->objects), shortcut_view_normal_face_codes[1], shortcut_view_normal_face_codes[2]);
	shortcut_new_scene = App->shortcut_manager->AddShortCut("New Scene", shortcut_new_scene_codes[0], std::bind(&PanelSceneSelector::OrganizeCreateNewScene, panel_scene_selector), shortcut_new_scene_codes[1], shortcut_new_scene_codes[2]);
	shortcut_save_scene_as_new = App->shortcut_manager->AddShortCut("Save Scene As New", shortcut_save_scene_as_new_codes[0], std::bind(&PanelSceneSelector::SaveSceneAsNew , panel_scene_selector), shortcut_save_scene_as_new_codes[1], shortcut_save_scene_as_new_codes[2]);
	shortcut_load_scene = App->shortcut_manager->AddShortCut("Load Scene", shortcut_load_scene_codes[0], std::bind(&PanelSceneSelector::OrganizeLoad, panel_scene_selector), shortcut_load_scene_codes[1], shortcut_load_scene_codes[2]);
	shortcut_outline = App->shortcut_manager->AddShortCut("Outline", shortcut_outline_codes[0], std::bind(&ModuleObjects::ChangeEnableOutline, App->objects), shortcut_outline_codes[1], shortcut_outline_codes[2]);
	shortcut_AABB = App->shortcut_manager->AddShortCut("AABB", shortcut_AABB_codes[0], std::bind(&ModuleObjects::ChangeEnableAABB, App->objects), shortcut_AABB_codes[1], shortcut_AABB_codes[2]);
	shortcut_OBB = App->shortcut_manager->AddShortCut("OBB", shortcut_OBB_codes[0], std::bind(&ModuleObjects::ChangeEnableOBB, App->objects), shortcut_OBB_codes[1], shortcut_OBB_codes[2]);
	shortcut_octree = App->shortcut_manager->AddShortCut("Octree", shortcut_octree_codes[0], std::bind(&ModuleObjects::ChangeEnableOctree, App->objects), shortcut_octree_codes[1], shortcut_octree_codes[2]);
	
	// OrderShortCuts must be called after all shortcuts have been created!! Victor read this...
	App->shortcut_manager->OrderShortCuts();
}

void ModuleUI::LoadActiveLayout()
{
	std::vector<Layout*>::iterator item = layouts.begin();
	for (; item != layouts.end(); ++item) {
		if (*item != nullptr && (*item)->active) {
			std::vector<Panel*>::iterator panel = panels.begin();
			for (; panel != panels.end(); ++panel) {
				if (*panel != nullptr) {
					(*panel)->SetEnable((*item)->panels_enabled[panel - panels.begin()]);
				}
			}
			ImGui::LoadIniSettingsFromDisk((*item)->path.data());
			break;
		}
	}
}

void ModuleUI::FramerateRegister(float frames, float ms)
{
	if (panel_config->IsEnabled())
		panel_config->FramerateInfo(frames,ms);
}

void ModuleUI::BackgroundDockspace()
{
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	// Seeting Docking to fit the window and preferences
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	/*if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;*/

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace BackGround", (bool*)0, window_flags);
	ImGui::PopStyleVar(3);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();
}

Layout::Layout(const char* name)
{
	this->name = std::string(name);
	number = App->ui->number_of_layouts += 1;
	
	path = std::string(CONFIGURATION_LAYOUTS_FOLDER) + name + std::to_string(number) + std::string(".ini");

	ImGui::SaveIniSettingsToDisk(path.data());
}

Layout::~Layout()
{
	
}
