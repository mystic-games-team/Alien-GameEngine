#include "PanelAbout.h"

PanelAbout::PanelAbout(const std::string& panel_name, const std::vector<SDL_Scancode>& shortcuts) : Panel(panel_name, shortcuts)
{
}

PanelAbout::~PanelAbout()
{
}

void PanelAbout::PanelLogic()
{
	ImGui::OpenPopup(panel_name.c_str());
	ImGui::SetNextWindowSize({ 500,700 });
	if (ImGui::BeginPopupModal(panel_name.c_str(), &enabled, ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove))
	{
		ImGui::Text("ALIEN ENGINE v0.1 :)");
		if (ImGui::Button("Github", { 50, 25 }))
		{
			App->OpenWebsite("https://github.com/VictorSegura99/Alien-GameEngine");
		}
		ImGui::Text("Alien Engine is the Videogame Power Engine of the future");
		ImGui::Text("By:");
		ImGui::Text("");
		ImGui::BulletText("Victor Segura Blanco");
		ImGui::SameLine(200.0f, 5.0f);
		if (ImGui::Button("Github Victor", { 50, 25 }))
		{
			App->OpenWebsite("https://github.com/VictorSegura99");
		}
		ImGui::BulletText("Oriol Capdevila Salinas");
		ImGui::SameLine(200.0f, 5.0f);
		if (ImGui::Button("Github Oriol", { 50, 25 }))
		{
			App->OpenWebsite("https://github.com/OriolCS2");
		}
		if (ImGui::CollapsingHeader("License"))
		{
			ImGui::Text("MIT License");
			ImGui::Text("Copyright (c) 2019 Victor Segura Blanco & Oriol Capdevila Salinas");
			ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :");
			ImGui::Text("MIT License");
			ImGui::Text("MIT License");
		}
		ImGui::EndPopup();
	}
}