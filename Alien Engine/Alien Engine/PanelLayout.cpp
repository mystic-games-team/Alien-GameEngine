#include "PanelLayout.h"
#include "Application.h"

PanelLayout::PanelLayout(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Edit Layout", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
}

PanelLayout::~PanelLayout()
{
}

void PanelLayout::PanelLogic()
{
	ImGui::OpenPopup(panel_name.c_str());
	ImGui::SetNextWindowSize({355, 200});
	if (ImGui::BeginPopupModal(panel_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::BeginChild("", { 345,0 }, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

		std::vector<Layout*>::iterator item = App->ui->layouts.begin();
		for (; item != App->ui->layouts.end(); ++item) {
			if (*item != nullptr) {

				ImGui::Text("Layout Name");
				ImGui::SameLine();

				ImGui::SetNextItemWidth(120);
				static char name[20];
				memcpy(name, (*item)->name.data(), 20);
				ImGui::PushID(*item);
				if (ImGui::InputText("##Layout Name", name, 20, ImGuiInputTextFlags_AutoSelectAll)) {
					(*item)->name = std::string(name);
				}
				ImGui::PopID();
				ImGui::SameLine();

				ImGui::PushID(item - App->ui->layouts.begin());
				if (ImGui::Button("Remove Layout")) {
					App->ui->DeleteLayout(*item);
				}
				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		ImGui::EndPopup();
	}


}
