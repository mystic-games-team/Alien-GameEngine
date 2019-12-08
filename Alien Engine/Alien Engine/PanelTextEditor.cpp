#include "PanelGame.h"
#include "Color.h"
#include "imgui/imgui.h"
#include "PanelTextEditor.h"
#include <fstream>

PanelTextEditor::PanelTextEditor(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Panel Text Editor", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
	
	auto lang = TextEditor::LanguageDefinition::CPlusPlus();

	// set your own known preprocessor symbols...
	{
		static const char* ppnames[] = { "NULL" };
		// ... and their corresponding values
		static const char* ppvalues[] = {
			"#define NULL 0"
		};

		for (int i = 0; i < sizeof(ppnames) / sizeof(ppnames[0]); ++i)
		{
			TextEditor::Identifier id;
			id.mDeclaration = ppvalues[i];
			lang.mPreprocIdentifiers.insert(std::make_pair(std::string(ppnames[i]), id));
		}
	}

	// set your own identifiers
	{
		static const char* identifiers[] = {
			"u64"
		};
		static const char* idecls[] =
		{
			"typedef unsigned long long u64"
		};
		for (int i = 0; i < sizeof(identifiers) / sizeof(identifiers[0]); ++i)
		{
			TextEditor::Identifier id;
			id.mDeclaration = std::string(idecls[i]);
			lang.mIdentifiers.insert(std::make_pair(std::string(identifiers[i]), id));
		}
	}

	text_editor.SetLanguageDefinition(lang);

	SetFile("AlienEngineScripts/Macros/AlienScripts.h");
}

PanelTextEditor::~PanelTextEditor()
{
}

void PanelTextEditor::PanelLogic()
{
	ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

	auto cpos = text_editor.GetCursorPosition();
	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, text_editor.GetTotalLines(),
		text_editor.IsOverwrite() ? "Ovr" : "Ins",
		text_editor.CanUndo() ? "*" : " ",
		text_editor.GetLanguageDefinition().mName.c_str(), "AlienEngineScripts/Hit.h");

	text_editor.Render("TextEditor");

	is_focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_ChildWindows);

	ImGui::End();
}

void PanelTextEditor::SetFile(const char* file)
{
	file_to_edit = std::string(file);

	std::ifstream t(file);
	if (t.good()) {
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		text_editor.SetText(str);
	}
}

bool PanelTextEditor::IsFocused() const
{
	if (enabled && is_focused) {
		return true;
	}
	return false;
}
