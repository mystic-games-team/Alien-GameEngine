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
		ImGui::Combo("", &objects_combo, "Cube\0Sphere\0Rock\0");

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