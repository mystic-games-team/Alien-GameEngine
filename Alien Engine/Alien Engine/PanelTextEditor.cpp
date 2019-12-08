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
		static const char* ppnames[] = { "NULL", "ALIEN_ENGINE_API", "ALIEN_INIT_DATA", "ALIEN_DESTROY_DATA", "ALIEN_INIT_HEADER",
			"ALIEN_END_HEADER", "ALIENENGINESCRIPTS_EXPORTS"
		};
		// ... and their corresponding values
		static const char* ppvalues[] = {
			"#define NULL 0", "#define ALIEN_ENGINE_API __declspec(dllexport)", "#define ALIEN_INIT_DATA ALIEN_ENGINE_API", "#define ALIEN_DESTROY_DATA ALIEN_ENGINE_API",
			"#define ALIEN_INIT_HEADER extern 'C' {", "#define ALIEN_END_HEADER }", "#define ALIENENGINESCRIPTS_EXPORTS", 
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
			"u64", "uint", "GameObject", "Alien", "Input"
		};
		static const char* idecls[] =
		{
			"typedef unsigned long long u64", "typedef unsigned int uint", "none", "none", "none"
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
	ImGui::Begin(panel_name.data(), &enabled, ImGuiWindowFlags_MenuBar);

	auto cpos = text_editor.GetCursorPosition();
	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				std::string to_save = text_editor.GetText();
				App->file_system->Save(file_to_edit.data(), to_save.data(), to_save.size());
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = text_editor.IsReadOnly();
			if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
				text_editor.SetReadOnly(ro);
			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && text_editor.CanUndo()))
				text_editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && text_editor.CanRedo()))
				text_editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, text_editor.HasSelection()))
				text_editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && text_editor.HasSelection()))
				text_editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && text_editor.HasSelection()))
				text_editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				text_editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				text_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(text_editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dark palette"))
				text_editor.SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				text_editor.SetPalette(TextEditor::GetLightPalette());
			if (ImGui::MenuItem("Retro blue palette"))
				text_editor.SetPalette(TextEditor::GetRetroBluePalette());
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}


	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, text_editor.GetTotalLines(),
		text_editor.IsOverwrite() ? "Ovr" : "Ins",
		text_editor.CanUndo() ? "*" : " ",
		text_editor.GetLanguageDefinition().mName.c_str(), file_name.data());

	text_editor.Render("TextEditor");

	is_focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_::ImGuiFocusedFlags_ChildWindows);

	ImGui::End();
}

void PanelTextEditor::SetFile(const char* file)
{
	file_to_edit = std::string(file);
	file_name = App->file_system->GetBaseFileNameWithExtension(file);

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
