#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include "ComponentCamera.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);

	void Movement();
	void Rotation();
	void Focus();
	void Zoom();

public:
	
	bool is_scene_hovered = false;
	bool is_scene_focused = false;

	float camera_speed = 30.0f;
	float camera_zoom_speed = 15.0f;
	float camera_mouse_speed = 10.0f;

	ComponentCamera* fake_camera = nullptr;

private:

	float3 newPos{ 0,0,0 };

	float speed = 0.f;
	float zoom_speed = 0.f;
	float mouse_speed = 0.f;

	GameObject* looking_at = nullptr;
	
	SDL_Cursor* cursor = nullptr;
};