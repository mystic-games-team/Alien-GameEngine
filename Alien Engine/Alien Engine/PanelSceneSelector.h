#pragma once

#include "Panel.h"

class PanelSceneSelector : public Panel {

public:

	enum class SceneSelectorState {
		SAVE_SCENE,
		SAVE_AS_NEW,
		LOAD_SCENE,
		CREATE_NEW_SCENE,

		NONE
	};

	PanelSceneSelector(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelSceneSelector();

	void PanelLogic();

	void OrganizeSave(const SceneSelectorState& state);

private:

	bool ExistsScene(const char* scene_name_with_extension);
	void SaveSceneAsNew();

private:

	SceneSelectorState state = SceneSelectorState::NONE;

};

