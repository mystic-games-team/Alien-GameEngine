#include "PanelSceneSelector.h"
#include "Color.h"
#include "imgui/imgui.h"
#include "ResourceTexture.h"
#include "imgui/imgui_internal.h"
#include "FileNode.h"

PanelSceneSelector::PanelSceneSelector(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra)
	: Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("Save", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
	enabled = false;
}

PanelSceneSelector::~PanelSceneSelector()
{
}

void PanelSceneSelector::PanelLogic()
{
	switch (state) {
	case SceneSelectorState::SAVE_SCENE:

		break;
	case SceneSelectorState::SAVE_AS_NEW:

		break;
	case SceneSelectorState::CREATE_NEW_SCENE:

		break;
	case SceneSelectorState::LOAD_SCENE:

		break;
	}
}

void PanelSceneSelector::OrganizeSave(const SceneSelectorState& state)
{
	switch (state) {
	case SceneSelectorState::SAVE_SCENE:

		break;
	case SceneSelectorState::SAVE_AS_NEW:

		break;
	case SceneSelectorState::CREATE_NEW_SCENE:

		break;
	case SceneSelectorState::LOAD_SCENE:

		break;
	}
}
