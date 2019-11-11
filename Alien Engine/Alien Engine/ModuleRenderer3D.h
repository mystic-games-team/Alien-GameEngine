#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "glew/include/glew.h"
#include "ModuleImporter.h"
#include "ComponentCamera.h"

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

	void RenderGrid();

	void ChangeDrawFrameBuffer(bool normal_frameBuffer);

public:

	// buffers to draw scene
	uint frame_buffer = 0;
	uint render_texture = 0;
	uint depthrenderbuffer = 0;

	// scene texture
	ResourceTexture* tex = nullptr;

	uint z_framebuffer;

	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	
	bool render_zbuffer = false;

public:

	ComponentCamera* actual_camera = nullptr;

	// grid
	int length_grid = 20;
	Color grid_color{ 1,1,1 };
	float grid_spacing = 1.f;
	int line_grid_width = 1;
};