#include "PanelConfig.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "imgui/imgui.h"
#include "mmgr/mmgr.h"
#include "gpudetect/DeviceId.h"

#include <Windows.h>

PanelConfig::PanelConfig(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra) :
	Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra), fps_keeper(100), ms_keeper(100)
{
	shortcut = App->shortcut_manager->AddShortCut("Panel Config", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
}

PanelConfig::~PanelConfig()
{
}

void PanelConfig::PanelLogic()
{
	ImGui::Begin("Configuration", &enabled, ImGuiWindowFlags_NoCollapse);

	if (ImGui::CollapsingHeader("Application"))
	{
		ImGui::Spacing();
		static char window_name[20];
		strcpy_s(window_name, 20, App->window->window_name);
		if (ImGui::InputText(" Application Name", window_name, 20)) {
			App->window->SetWindowName(window_name);
		}
		ImGui::Spacing();
		static char organitzation_name[20];
		strcpy_s(organitzation_name, 20, App->window->organitzation_name);
		if (ImGui::InputText(" Organitzation Name", organitzation_name, 20)) {
			App->window->SetOrganitzationName(organitzation_name);
		}
		ImGui::Spacing();
		if (ImGui::SliderInt(" Max FPS", &App->fps_limit, 0, 120)) {
			if (App->fps_limit <= 0) {
				App->fps_cap = false;
			}
			else {
				App->fps_cap = true;
				App->framerate_cap = 1000 / App->fps_limit;
			}
		}
		ImGui::Spacing();
		if (App->fps_limit <= 0)
			ImGui::Text("Limit FPS: No Limit", App->fps_limit);
		else
			ImGui::Text("Limit FPS: %i", App->fps_limit);
		ImGui::Spacing();

		// Framerate Histogram
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_keeper[fps_keeper.size() - 1]);
		ImGui::PlotHistogram("", &fps_keeper[0], fps_keeper.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

		// Ms Histogram
		sprintf_s(title, 25, "Ms %.1f", ms_keeper[ms_keeper.size() - 1]);
		ImGui::PlotHistogram("", &ms_keeper[0], ms_keeper.size(), 0, title, 0.0f, 60.0f, ImVec2(310, 100));

		// Memory Histogram
		sMStats memory_stats = m_getMemoryStatistics();
		static int memory_count = 0;
		static std::vector<float> memory(100);
		if (++memory_count < 20)
		{
			memory_count = 0;
			if (memory.size() == 100)
			{
				for (uint i = 0; i < 99; i++)
				{
					memory[i] = memory[i + 1];
				}
				memory[100 - 1] = memory_stats.totalReportedMemory;
			}
			else
			{
				memory.push_back(memory_stats.totalReportedMemory);
			}
		}

		ImGui::PlotHistogram("", &memory[0], memory.size(), 0, "Memory Use", 0.0f, (float)memory_stats.peakReportedMemory*1.2f, ImVec2(310, 100));
		

		ImGui::Text("Total Reported Memory: %u", memory_stats.totalReportedMemory);
		ImGui::Text("Total Actual Memory: %u", memory_stats.totalActualMemory);
		ImGui::Text("Max Reported Memory: %u", memory_stats.peakReportedMemory);
		ImGui::Text("Max Actual Mem: %u", memory_stats.peakActualMemory);
		ImGui::Text("Accumulated Reported Mem: %u", memory_stats.accumulatedReportedMemory);
		ImGui::Text("Accumulated Actual Mem: %u", memory_stats.accumulatedActualMemory);
		ImGui::Text("Accumulated Alloc Unit Count: %u", memory_stats.accumulatedAllocUnitCount);
		ImGui::Text("Total Alloc Unit Count: %u", memory_stats.totalAllocUnitCount);
		ImGui::Text("Peak Alloc Unit Count: %u", memory_stats.peakAllocUnitCount);


		ImGui::Spacing();
	}
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Spacing();
		if (ImGui::SliderInt(" Width", &App->window->width, 0, 1920)) {
			SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
		}
		if (ImGui::SliderInt(" Height", &App->window->height, 0, 1080)) {
			SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
		}
		if (ImGui::SliderFloat(" Brightness", &App->window->brightness, 0.0, 1.0)) {
			SDL_SetWindowBrightness(App->window->window, App->window->brightness);
		}
		ImGui::Spacing();
		SDL_DisplayMode display_mode;
		SDL_GetDesktopDisplayMode(0, &display_mode);
		ImGui::Text("Refresh Rate:"); ImGui::SameLine(); ImGui::TextColored({ 255,0,0,255 }, "%i", display_mode.refresh_rate);
		ImGui::Spacing();
		if (ImGui::Checkbox(" FullScreen", &App->window->fullscreen)) {
			if (App->window->fullscreen) {
				SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
			}
			else {
				SDL_SetWindowFullscreen(App->window->window, 0);
			}
		}
		ImGui::SameLine();
		if (ImGui::Checkbox(" FullDesktop", &App->window->full_desktop)) {
			if (App->window->full_desktop) {
				SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
			else {
				SDL_SetWindowFullscreen(App->window->window, 0);
			}
		}
		if (ImGui::Checkbox(" Borderless", &App->window->borderless)) {
			SDL_SetWindowBordered(App->window->window, (SDL_bool)!App->window->borderless);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox(" Resizable", &App->window->resizable)) {
			SDL_SetWindowResizable(App->window->window, (SDL_bool)App->window->resizable);
		}
		
		ImGui::Spacing();
	}
	if (ImGui::CollapsingHeader("Style")) {
		ImGui::Spacing();
		ImGui::Text("Select the style");
		ImGui::Spacing();
		if (ImGui::Combo("", &App->window->style, "Classic\0Dark\0Light\0"))
		{
			switch (App->window->style)
			{
			case 0: ImGui::StyleColorsClassic(); break;
			case 1: ImGui::StyleColorsDark(); break;
			case 2: ImGui::StyleColorsLight(); break;
			}
		}
		ImGui::Spacing();
	}
	if (ImGui::CollapsingHeader("ShortCuts")) {
		ImGui::Spacing();
		ImGui::BeginChild("", { 440,0 },true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::Columns(4, 0, false);
		ImGui::SetColumnWidth(0,110);
		ImGui::SetColumnWidth(1, 100);
		ImGui::SetColumnWidth(2, 100);
		ImGui::SetColumnWidth(3, 130);
		//ImGui::Text("");
		ImGui::NextColumn();
		ImGui::Text("OnKeyRepeat");
		ImGui::NextColumn();
		ImGui::Text("OnKeyDown");
		ImGui::NextColumn();
		ImGui::Text("ExtraKeyRepeat");
		ImGui::NextColumn();
		std::vector<ShortCut*> shortcuts = App->shortcut_manager->GetShortCuts();
		std::vector<ShortCut*>::iterator item = shortcuts.begin();
		for (; item != shortcuts.end(); ++item) {
			if (*item != nullptr) {
				ImGui::Spacing();
				ImGui::Text((*item)->GetNameOrder());
				ImGui::NextColumn();
				ImGui::Spacing();
				ImGui::Button((*item)->GetKeyRepeatName(), { 75,30 });
				if (ImGui::IsItemClicked()) {
					(*item)->state = ShortCutStateChange::WAITING_KEY_REPEAT;
				}
				ImGui::NextColumn();
				ImGui::Spacing();
				ImGui::Button((*item)->GetKeyDownName(), { 75,30 });
				if (ImGui::IsItemClicked()) {
					(*item)->state = ShortCutStateChange::WAITING_KEY_DOWN;
				}
				ImGui::NextColumn();
				ImGui::Spacing();
				ImGui::Button((*item)->GetExtraKeyRepeatName(), { 75,30 });
				if (ImGui::IsItemClicked()) {
					(*item)->state = ShortCutStateChange::WAITING_EXTRA_KEY_REPEAT;
				}
				ImGui::NextColumn();
			}
		}
		ImGui::EndChild();
	}
	if (ImGui::CollapsingHeader("Input")) {
		ImGui::Spacing();
		ImGui::Text("Mouse Position:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,216,0,100 }, "%i, %i", App->input->GetMouseX(), App->input->GetMouseY());
		ImGui::Text("Mouse Motion:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,216,0,100 }, "%i, %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
		ImGui::Text("Mouse Wheel:");
		ImGui::SameLine();
		ImGui::TextColored({ 255,216,0,100 }, "%i", App->input->GetMouseZ());
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::BeginChild("Input Output", { 0, 150 });
		ImGui::TextUnformatted(App->input->input.begin());
		if (scroll_y) {
			scroll_y = false;
			ImGui::SetScrollHereY(1.0F);
		}

		if (ImGui::GetScrollY() / ImGui::GetScrollMaxY() >= 0.8F) {
			ImGui::SetScrollHereY(1.0F);
		}
		ImGui::EndChild();

		
	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		std::wstring gpu;
		char gpu_brand[250] = "";
		unsigned __int64 video_mem_budget;
		unsigned __int64 video_mem_usage;
		unsigned __int64 video_mem_available;
		unsigned __int64 video_mem_reserved;

		getGraphicsDeviceInfo(NULL,NULL, &gpu, &video_mem_budget, &video_mem_usage, &video_mem_available, &video_mem_reserved);

		ImGui::Text("OS: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%s", SDL_GetCurrentVideoDriver());
		ImGui::Text("CPUs: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i Cores  ", SDL_GetCPUCount());  ImGui::SameLine(); ImGui::Text("Cache: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i kb", SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%.2fGB", (float(SDL_GetSystemRAM()) / 1024.f));
		ImGui::Text("Caps: "); ImGui::SameLine(); if (SDL_Has3DNow())ImGui::TextColored({ 255,216,0,100 }, "3D, "); ImGui::SameLine(); if (SDL_HasAVX())ImGui::TextColored({ 255,216,0,100 }, "AVX, "); ImGui::SameLine();  if (SDL_HasAVX2())ImGui::TextColored({ 255,216,0,100 }, "AVX2, ");
		ImGui::SameLine();  if (SDL_HasAltiVec())ImGui::TextColored({ 255,216,0,100 }, "AltiVec, "); ImGui::SameLine();  if (SDL_HasMMX())ImGui::TextColored({ 255,216,0,100 }, "MMX, ");
		ImGui::SameLine();  if (SDL_HasRDTSC())ImGui::TextColored({ 255,216,0,100 }, "RDTSC, "); ImGui::SameLine();  if (SDL_HasSSE())ImGui::TextColored({ 255,216,0,100 }, "SSE, "); ImGui::SameLine();  if (SDL_HasSSE2())ImGui::TextColored({ 255,216,0,100 }, "SSE2, ");
		if (SDL_HasSSE3())ImGui::TextColored({ 255,216,0,100 }, "SSE3, "); ImGui::SameLine();  if (SDL_HasSSE41())ImGui::TextColored({ 255,216,0,100 }, "SSE41, "); ImGui::SameLine();  if (SDL_HasSSE42())ImGui::TextColored({ 255,216,0,100 }, "SSE42 ");
		ImGui::Separator();
		sprintf_s(gpu_brand, 250, "%S", gpu.c_str());
		ImGui::Text("GPU: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, gpu_brand);
		ImGui::Text("VRAM Budget: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i Mb", (float(video_mem_budget)/(1024.f * 1024.f)));
		ImGui::Text("VRAM Usage: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i Mb", (float(video_mem_usage) / (1024.f * 1024.f )));
		ImGui::Text("VRAM Available: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i Mb", (float(video_mem_available)/ (1024.f * 1024.f)));
		ImGui::Text("VRAM Reserved: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i Mb", (float(video_mem_reserved)/ (1024.f * 1024.f)));
		ImGui::Spacing();
	}
	ImGui::Spacing();
	if (ImGui::Button("Save Configuration", { 150,30 })) {
		App->SaveCustomConfig();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Default Configuration", { 200,30 })) {
		App->LoadDefaultConfig();
	}
	ImGui::End();
}

void PanelConfig::FramerateInfo(float frames, float ms)
{
	static uint count = 0;

	if (count == 100)
	{
		for (uint i = 0; i < 100 - 1; ++i)
		{
			fps_keeper[i] = fps_keeper[i + 1];
			ms_keeper[i] = ms_keeper[i + 1];
		}
	}
	else
		++count;

	fps_keeper[count - 1] = frames;
	ms_keeper[count - 1] = ms;
}

void PanelConfig::OnPanelDesactive()
{
	scroll_y = true;
}

