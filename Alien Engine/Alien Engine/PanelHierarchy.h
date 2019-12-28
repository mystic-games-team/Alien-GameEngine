#pragma once

#include "Panel.h"
#include "GameObject.h"

class PanelHierarchy : public Panel {

public:

	PanelHierarchy(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat = SDL_SCANCODE_UNKNOWN, const SDL_Scancode& key3_repeat_extra = SDL_SCANCODE_UNKNOWN);
	virtual ~PanelHierarchy();

	void PanelLogic();

	bool is_focused = false;
	bool popup_no_open_prefab = false;
private:

	void PrintNode(GameObject* node);
	void RightClickMenu();


private:

	GameObject* object_hovered = nullptr;
	GameObject* object_menu = nullptr;
	bool in_menu = false;

	bool popup_prefab_restructurate = false;
	bool popup_leave_prefab_view = false;
	bool popup_delete_root_prefab_scene = false;
	bool popup_move_child_outof_root_prefab_scene = false;

};
