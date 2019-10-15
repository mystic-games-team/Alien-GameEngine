#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "glew/include/glew.h"
#include "ModuleImporter.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void CreateRenderTexture();

	void SetBackgroundColor(const Color & bg_color);

	void RenderGrid();


public:
	uint frame_buffer = 0;
	uint render_texture = 0;
	uint depthrenderbuffer = 0;
	uint stencil_buffer = 0;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	Texture* tex = nullptr;
public:

	Color background_color{ 0.f, 0.f, 0.f, 1.f };
	int length_grid = 20;
	Color grid_color{ 1,1,1 };
	float grid_spacing = 1.f;
	int line_grid_width = 1;
};