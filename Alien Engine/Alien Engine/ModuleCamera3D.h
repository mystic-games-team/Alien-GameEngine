#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);

	void Movement();
	void Rotation();
	void Focus();
	void Zoom();

	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;

	float camera_speed = 30.0f;
	float camera_zoom_speed = 15.0f;
	float camera_mouse_speed = 15.0f;
private:

	mat4x4 ViewMatrix, ViewMatrixInverse;

	vec3 newPos{ 0,0,0 };
	float speed = 0.f;
	float zoom_speed = 0.f;
	float mouse_speed = 0.f;


};