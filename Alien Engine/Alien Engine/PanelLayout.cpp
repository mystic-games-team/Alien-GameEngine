#include "PanelLayout.h"
#include "Application.h"

PanelLayout::PanelLayout(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Edit Layout", key1_down, std::bind(&PanelLayout::ChangePanel, this), key2_repeat, key3_repeat_extra);
}

PanelLayout::~PanelLayout()
{
}

void PanelLayout::PanelLogic()
{
	if (is_editor_panel) {
		PanelLayoutEditor();
	}
	else {
		PanelSaveNewLayout();
	}
}

void PanelLayout::ChangePanel()
{
	enabled = !enabled;
	is_editor_panel = true;
}

void PanelLayout::PanelLayoutEditor()
{
	ImGui::OpenPopup(panel_name.c_str());
	ImGui::SetNextWindowSize({ 270, 200 });
	if (ImGui::BeginPopupModal(panel_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::BeginChild("", { 260,0 }, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

		ImGui::Text("");
		ImGui::SameLine(23);
		ImGui::Text("Layout Name");
		ImGui::Spacing();
		std::vector<Layout*>::iterator item = App->ui->layouts.begin();
		for (; item != App->ui->layouts.end(); ++item) {
			if (*item != nullptr) {
				
				if ((*item)->name == "Default")
					continue;

				ImGui::Text("");
				ImGui::SameLine(5);
				ImGui::SetNextItemWidth(120);
				static char name[20];
				memcpy(name, (*item)->name.data(), 20);
				ImGui::PushID(*item);
				if (ImGui::InputText("##Layout Name", name, 20, ImGuiInputTextFlags_AutoSelectAll)) {
					(*item)->name = std::string(name);
				}
				ImGui::PopID();
				ImGui::SameLine(142);

				ImGui::PushID(item - App->ui->layouts.begin());
				if (ImGui::Button("Remove Layout")) {
					ImGui::PopID();
					ImGui::EndChild();
					ImGui::EndPopup();
					if ((*item) == App->ui->active_layout) {
						App->ui->ResetImGui();
						App->ui->DeleteLayout(*item);
						App->ui->LoadActiveLayout();
						ImGui::NewFrame();
					}
					
					return;
				}
				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		ImGui::EndPopup();
	}
}

void PanelLayout::PanelSaveNewLayout()
{
	ImGui::OpenPopup(panel_name.c_str());
	ImGui::SetNextWindowSize({ 218, 85 });
	if (ImGui::BeginPopupModal(panel_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::SetNextItemWidth(120);
		static char name[20];
		memcpy(name, new_layout_name.data(), 20);
		if (ImGui::InputText("Layout Name", name, 20, ImGuiInputTextFlags_AutoSelectAll)) {
			new_layout_name = std::string(name);
		}
		ImGui::Spacing();
		ImGui::NewLine();
		ImGui::SameLine(80);
		if (ImGui::Button("Create")) {
			ChangeEnable();
			Layout* layout = new Layout(new_layout_name.data());
			App->ui->layouts.push_back(layout);
			layout->active = true;
			App->ui->active_layout->active = false;
			App->ui->active_layout = layout;
			App->ui->SaveLayout(layout);
			new_layout_name = "New Layout";
		}
		ImGui::EndPopup();
	}
}
