#include "PanelConfig.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "imgui/imgui.h"

#include <Windows.h>

PanelConfig::PanelConfig(const std::string& panel_name, const std::vector<SDL_Scancode>& shortcuts) : Panel(panel_name, shortcuts)
{
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
		if (ImGui::InputText("Application Name", App->window->window_name, 20)) {
			SDL_SetWindowTitle(App->window->window, App->window->window_name);
		}
		ImGui::Spacing();
		ImGui::InputText("Organitzation Name", App->window->organitzation_name, 20);
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
		ImGui::Text("Refresh Rate:"); ImGui::SameLine(); ImGui::TextColored({ 255,0,0,255 }, "TODOOOOOOOOOOOOOOOOOOO SAD");
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
	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("OS: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%s", SDL_GetCurrentVideoDriver());
		ImGui::Text("CPUs: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i Cores  ", SDL_GetCPUCount());  ImGui::SameLine(); ImGui::Text("Cache: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i kb", SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%.2fGB", (float(SDL_GetSystemRAM()) / 1000));
		ImGui::Text("Caps: "); ImGui::SameLine(); if (SDL_Has3DNow)ImGui::TextColored({ 255,216,0,100 }, "3D, "); ImGui::SameLine(); if (SDL_HasAVX)ImGui::TextColored({ 255,216,0,100 }, "AVX, "); ImGui::SameLine();  if (SDL_HasAVX2)ImGui::TextColored({ 255,216,0,100 }, "AVX2, ");
		ImGui::SameLine();  if (SDL_HasAltiVec)ImGui::TextColored({ 255,216,0,100 }, "AltiVec, "); ImGui::SameLine();  if (SDL_HasMMX)ImGui::TextColored({ 255,216,0,100 }, "MMX, ");
		ImGui::SameLine();  if (SDL_HasRDTSC)ImGui::TextColored({ 255,216,0,100 }, "RDTSC, "); ImGui::SameLine();  if (SDL_HasSSE)ImGui::TextColored({ 255,216,0,100 }, "SSE, "); ImGui::SameLine();  if (SDL_HasSSE2)ImGui::TextColored({ 255,216,0,100 }, "SSE2, ");
		if (SDL_HasSSE3)ImGui::TextColored({ 255,216,0,100 }, "SSE3, "); ImGui::SameLine();  if (SDL_HasSSE41)ImGui::TextColored({ 255,216,0,100 }, "SSE41, "); ImGui::SameLine();  if (SDL_HasSSE42)ImGui::TextColored({ 255,216,0,100 }, "SSE42 ");
		ImGui::Separator();
		ImGui::Text("GPU Brand: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%s", glGetString(GL_VENDOR));
		ImGui::Text("GPU: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%s", glGetString(GL_RENDERER));
		ImGui::Text("VRAM: "); ImGui::SameLine(); ImGui::TextColored({ 255,216,0,100 }, "%i GB",sizeof(LPMEMORYSTATUSEX));
		ImGui::Spacing();
	}
	ImGui::End();
}