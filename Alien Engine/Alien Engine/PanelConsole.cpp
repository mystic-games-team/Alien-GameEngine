#include "PanelConsole.h"

PanelConsole::PanelConsole(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra) :
	Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Panel Console", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
}

PanelConsole::~PanelConsole()
{
}

void PanelConsole::PanelLogic()
{

	ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_NoCollapse);
	
	ImGui::BeginChild("#console buttons", { ImGui::GetWindowWidth(),30 });
	if (!game_console) ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.06F, 0.53F,0.98F,1 });
	if (ImGui::Button("Engine Console") && game_console) {
		game_console = false;
	}
	else if (!game_console) ImGui::PopStyleColor();
	ImGui::SameLine();
	if (game_console) ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.06F, 0.53F,0.98F,1 });
	if (ImGui::Button("Game Console") && !game_console) {
		game_console = true;
	}
	else if (game_console) ImGui::PopStyleColor();
	ImGui::SameLine();
	ImGui::Text("|");
	ImGui::SameLine();
	if (ImGui::Button("Clear")) {
		if (game_console) App->game_logs.clear();
		else App->engine_logs.clear();
	}
	ImGui::SameLine();

	if (clear_on_play) ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.06F, 0.53F,0.98F,1 });
	if (ImGui::Button("Clear On Play")) {
		if (clear_on_play) ImGui::PopStyleColor();
		clear_on_play = !clear_on_play;
	}
	else if (clear_on_play) ImGui::PopStyleColor();

	ImGui::Separator();
	ImGui::EndChild();
	ImGui::BeginChild("#console logs", { 0,0 },false, ImGuiWindowFlags_HorizontalScrollbar);

	if (game_console) ImGui::TextUnformatted(App->game_logs.begin());
	else ImGui::TextUnformatted(App->engine_logs.begin());
	

	if (scroll_x) {
		scroll_x = false;
		ImGui::SetScrollHereX(1.0F);
	}
	if (scroll_y) {
		ImGui::SetScrollHereY(1.0F);
	}

	if (!scroll_y && ImGui::GetScrollY() / ImGui::GetScrollMaxY() >= 0.95F) {
		scroll_y = true;
	}
	else if (scroll_y && ImGui::GetScrollY() / ImGui::GetScrollMaxY() <= 0.95F) {
		scroll_y = false;
	}

	if (ImGui::IsWindowHovered()) {
		App->camera->is_scene_hovered = false;
	}
	ImGui::EndChild();

	ImGui::End();
}

void PanelConsole::OnPanelDesactive()
{
	scroll_x = true;
	scroll_y = true;
}
