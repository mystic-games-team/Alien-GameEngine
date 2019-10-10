#include "Application.h"
#include "PanelCreateObject.h"
#include "Objects.h"
#include "ModuleObjects.h"
#include "Primitive.h"

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
	ImGui::SetNextWindowContentWidth(200);
	if (ImGui::BeginPopupModal(panel_name.c_str(), &enabled, ImGuiWindowFlags_NoResize| ImGuiWindowFlags_AlwaysAutoResize))
	{
		LOG("%f", ImGui::GetWindowWidth());
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::PushItemWidth(100);
		ImGui::Text("Object:  "); ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::Combo("", &objects_combo, "Cube\0Sphere\0Rock\0Dodecahedron\0Icosahedron\0Octahedron\0Torus\0");

		ImGui::Spacing();
		ImGui::Spacing();


		ImGui::Columns(3,0,false);
		ImGui::Text("X:");  ImGui::SameLine(0, 0); ImGui::PushItemWidth(40); ImGui::InputFloat("##x", &x, 0, 0, 2);
		ImGui::NextColumn();
		ImGui::Text("Y:"); ImGui::SameLine(0,0); ImGui::PushItemWidth(40); ImGui::InputFloat("##y", &y, 0, 0, 2);
		ImGui::NextColumn();
		ImGui::Text("Z:"); ImGui::SameLine(0,0); ImGui::PushItemWidth(40); ImGui::InputFloat("##z", &z, 0, 0, 2);
		ImGui::Columns(1);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Choose Color"); ImGui::SameLine();
		ImGui::ColorEdit3("##colorcreate", (float*)& create_color, ImGuiColorEditFlags_Float| ImGuiColorEditFlags_NoInputs| ImGuiColorEditFlags_NoLabel);

		ImGui::Spacing();
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
			ImGui::Spacing();
			ImGui::Spacing();
			break;
		case 2:
			ImGui::Text("Subdivions:"); ImGui::SameLine();
			ImGui::SliderInt("##slider int", &subdivions, 1, 5);

			ImGui::Text("Seed:"); ImGui::SameLine();
			ImGui::InputInt("##input int", &seed,0,0);
			ImGui::Spacing();
			ImGui::Spacing();
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

		LOG("%f", ImGui::GetWindowWidth());

		if (ImGui::Button("Create", { ImGui::GetWindowWidth()-16,25 }))
		{
			Object* object;
			switch (objects_combo)
			{
			case 0:
				object=App->objects->CreatePrimitive(PrimitiveType::CUBE, x,y,z);
				x = y = z = 0;
				break;
			case 1:
				object=App->objects->CreatePrimitive(PrimitiveType::SPHERE_ALIEN, x, y, z,subdivions);
				x = y = z = 0;
				subdivions = 5;
				break;
			case 2:
				object=App->objects->CreatePrimitive(PrimitiveType::ROCK, x, y, z,subdivions,seed);
				x = y = z = seed = 0;
				subdivions = 5;
				break;
			case 3:
				object = App->objects->CreatePrimitive(PrimitiveType::DODECAHEDRON, x, y, z);
				x = y = z = 0;
				break;
			case 4:
				object = App->objects->CreatePrimitive(PrimitiveType::ICOSAHEDRON, x, y, z);
				x = y = z = 0;
				break;
			case 5:
				object = App->objects->CreatePrimitive(PrimitiveType::OCTAHEDRON, x, y, z);
				x = y = z = 0;
				break;
			case 6:
				object = App->objects->CreatePrimitive(PrimitiveType::TORUS, x, y, z,NULL,NULL,slices,stacks,radius);
				x = y = z = 0;
				slices = stacks = 5;
				radius = 0.5f;
				break;
			}

			object->SetColor(create_color);
			create_color = { 1,1,1 };

			ChangeEnable();
		}


		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::EndPopup();
	}
}