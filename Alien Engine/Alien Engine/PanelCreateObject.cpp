#include "Application.h"
#include "PanelCreateObject.h"
#include "GameObject.h"
#include "ModuleObjects.h"
#include "Shapes.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

PanelCreateObject::PanelCreateObject(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra) :
	Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("CreateObject", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
}

PanelCreateObject::~PanelCreateObject()
{
}

void PanelCreateObject::PanelLogic()
{
	static float x = 0.0f;
	static float y = 0.f;
	static float z = 0.f;
	static int subdivions = 5;
	static int seed = 0;

	static int slices = 5;
	static int stacks = 5;
	static float radius = 0.5f;

	static int objects_combo = 0;

	ImGui::OpenPopup(panel_name.c_str());
	ImGui::SetNextWindowContentWidth(300);
	if (ImGui::BeginPopupModal(panel_name.c_str(), &enabled, ImGuiWindowFlags_NoResize| ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::IsWindowHovered())
			App->camera->is_scene_hovered = false;
		ImGui::Spacing();
		ImGui::Spacing();

		
		ImGui::Combo("Select Object", &objects_combo, "Cube\0Sphere\0Rock\0Dodecahedron\0Icosahedron\0Octahedron\0Torus\0");

		ImGui::Spacing();
		
		ImGui::ColorEdit3("Select Color", (float*)&create_color, ImGuiColorEditFlags_Float);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Position");
		ImGui::Columns(3,0,false);
		ImGui::Text("   X:");  ImGui::SameLine(0, 0); ImGui::PushItemWidth(40); ImGui::InputFloat("##x", &x, 0, 0, 2);
		ImGui::NextColumn();
		ImGui::Text("Y:"); ImGui::SameLine(0,0); ImGui::PushItemWidth(40); ImGui::InputFloat("##y", &y, 0, 0, 2);
		ImGui::NextColumn();
		ImGui::Text("Z:"); ImGui::SameLine(0,0); ImGui::PushItemWidth(40); ImGui::InputFloat("##z", &z, 0, 0, 2);
		ImGui::NextColumn();
		ImGui::Columns(1);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		switch (objects_combo)
		{
		case 0: 
			break;
		case 1:
			ImGui::Text("Subdivions:"); ImGui::SameLine();
			ImGui::SliderInt("##subdivisions", &subdivions, 1, 5);
			break;
		case 2:
			ImGui::Text("Subdivions:"); ImGui::SameLine();
			ImGui::SliderInt("##slider int", &subdivions, 1, 5);

			ImGui::Text("Seed:"); ImGui::SameLine();
			ImGui::InputInt("##input int", &seed,0,0);
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			ImGui::Text("Slices"); ImGui::SameLine();
			ImGui::SliderInt("##Slices Slider", &slices, 3, 20);
			ImGui::Text("Stacks"); ImGui::SameLine();
			ImGui::SliderInt("##Stacks Slider", &stacks, 3, 20);
			ImGui::Text("Radius"); ImGui::SameLine();
			ImGui::SliderFloat("##Radius Slider", &radius, 0.1f, 1);

			break;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Create", { ImGui::GetWindowWidth()-16,25 }))
		{
			GameObject* object = new GameObject();
			object->parent = App->objects->base_game_object;
			App->objects->base_game_object->AddChild(object);
			ComponentTransform* transform = new ComponentTransform(object, { x,y,z }, { 0,0,0,0 }, { 1,1,1 });
			ComponentMesh* mesh = new ComponentMesh(object);
			ComponentMaterial* material = new ComponentMaterial(object);
			par_shapes_mesh* par_mesh = nullptr;
			switch (objects_combo)
			{
			case 0:
				par_mesh = par_shapes_create_cube();
				object->SetName("Cube");
				x = y = z = 0;
				break;
			case 1:
				par_mesh = par_shapes_create_subdivided_sphere(subdivions);
				object->SetName("Sphere");
				x = y = z = 0;
				subdivions = 5;
				break;
			case 2:
				par_mesh = par_shapes_create_rock(seed, subdivions);
				object->SetName("Rock");
				x = y = z = seed = 0;
				subdivions = 5;
				break;
			case 3:
				par_mesh = par_shapes_create_dodecahedron();
				object->SetName("Dodecahedron");
				x = y = z = 0;
				break;
			case 4:
				par_mesh = par_shapes_create_icosahedron();
				object->SetName("Icosahedron");
				x = y = z = 0;
				break;
			case 5:
				par_mesh = par_shapes_create_octahedron();
				object->SetName("Octahedron");
				x = y = z = 0;
				break;
			case 6:
				par_mesh = par_shapes_create_torus(slices, stacks, radius);
				object->SetName("Torus");
				x = y = z = 0;
				slices = stacks = 5;
				radius = 0.5f;
				break;
			}

			App->importer->LoadParShapesMesh(par_mesh, mesh);
			material->color = create_color;
			object->AddComponent(transform);
			object->AddComponent(mesh);
			object->AddComponent(material);
			App->objects->SetNewSelectedObject(object);
			par_shapes_free_mesh(par_mesh);

			create_color = { 1,1,1 };
			ChangeEnable();
		}


		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::EndPopup();
	}
}