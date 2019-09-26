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
	ImGui::SetNextWindowSize({ 500,600 });
	if (ImGui::BeginPopupModal(panel_name.c_str(), &enabled, ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove))
	{
		ImGui::Text("ALIEN ENGINE %f", ALIEN_ENGINE_VERSION);
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
		ImGui::Text("Libraries Versions:");
		ImGui::BulletText("SDL %i.%i.%i", SDL_MAJOR_VERSION,SDL_MINOR_VERSION,SDL_PATCHLEVEL);
		ImGui::BulletText("Glew %s", VERSION_GLEW);
		ImGui::BulletText("ImGui %s", IMGUI_VERSION);
		ImGui::BulletText("MathGeoLib 1.5");
		ImGui::BulletText("OpenGL %s",glGetString(GL_VERSION));
		ImGui::Text("");
		if (ImGui::CollapsingHeader("License"))
		{
			ImGui::Text("MIT License");
			ImGui::Text("Copyright (c) 2019 Victor Segura Blanco & Oriol Capdevila Salinas");
			ImGui::Text("");
			ImGui::Text("Permission is hereby granted, free of charge, to any person \nobtaining a copy of this software and associated documentation \nfiles(the 'Software'), to deal in the Software without restriction, \nincluding without limitation the rights to use, copy, modify, merge, \npublish, distribute, sublicense, and /or sell copies of the Software, \nand to permit persons to whom the Software is furnished to do so, \nsubject to the following conditions :");
			ImGui::Text("");
			ImGui::Text("The above copyright notice and this permission notice shall \nbe included in all copies or substantial portions of the Software.");
			ImGui::Text("");
			ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY \nKIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES \nOF MERCHANTABILITY,FITNESS FOR A PARTICULAR PURPOSE AND \nNONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS \nBE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN \nACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN \nCONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN \nTHE SOFTWARE.");
		}
		ImGui::EndPopup();
	}
}