#include "Application.h"
#include "PanelCreateObject.h"
#include "Objects.h"
#include "ModuleObjects.h"

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
	static int subdivions = 0;
	static int seed = 0;

	static int objects_combo = 0;

	ImGui::OpenPopup(panel_name.c_str());
	ImGui::SetNextWindowSize({ 300,150 });
	if (ImGui::BeginPopupModal(panel_name.c_str(), &enabled, ImGuiWindowFlags_NoResize))
	{
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Columns(3,0,false);
		ImGui::SetColumnWidth(0, 100);
		ImGui::SetColumnWidth(1, 80);
		ImGui::SetColumnWidth(2, 80);
		ImGui::Text("");  ImGui::SameLine(0,20);
		ImGui::Text("X:");  ImGui::SameLine(0, 0); ImGui::PushItemWidth(60); ImGui::InputFloat("x", &x, 0, 0, 2);
		ImGui::NextColumn();
		ImGui::Text("Y:"); ImGui::SameLine(0,0); ImGui::PushItemWidth(60); ImGui::InputFloat("y", &y, 0, 0, 2);
		ImGui::NextColumn();
		ImGui::Text("Z:"); ImGui::SameLine(0,0); ImGui::PushItemWidth(60); ImGui::InputFloat("z", &z, 0, 0, 2);
		ImGui::Columns(1);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Separator();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Object:  "); ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::Combo("", &objects_combo, "Cube\0Sphere\0Rock\0");

		ImGui::SameLine(0,30);
		if (ImGui::Button("Create", { 50,20 }))
		{
			switch (objects_combo)
			{
			case 0:
				App->objects->CreatePrimitive(PrimitiveType::CUBE, x, y, z);
				break;
			case 1:
				App->objects->CreatePrimitive(PrimitiveType::SPHERE_ALIEN, x, y, z, subdivions);
				break;
			case 2: 
				App->objects->CreatePrimitive(PrimitiveType::ROCK, x, y, z, subdivions, seed);
				break;
			}

			
			ChangeEnable();
		}

		ImGui::Spacing();
		ImGui::Spacing();

		switch (objects_combo)
		{
		case 0: 
			break;
		case 1:
			ImGui::Text("Subdivions:"); ImGui::SameLine();
			ImGui::SliderInt("", &subdivions, 1, 5);
			break;
		case 2:
			ImGui::Text("Subdivions:"); ImGui::SameLine();
			ImGui::SliderInt(".", &subdivions, 1, 5);

			ImGui::Text("Seed:"); ImGui::SameLine();
			ImGui::InputInt("", &seed,0,0);
			break;
		}

		ImGui::EndPopup();
	}
}