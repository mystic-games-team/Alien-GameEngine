#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ModuleUI.h"
#include "ResourceTexture.h"
#include "ComponentCamera.h"
#include "ModuleCamera3D.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "mmgr/mmgr.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glew/glew32.lib") 



ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	name.assign("Renderer3D");
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG_ENGINE("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG_ENGINE("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	glewInit();

#ifndef GAME_VERSION
	App->camera->fake_camera = new ComponentCamera(nullptr);
	App->camera->fake_camera->frustum.farPlaneDistance = 1000.0F;
	scene_fake_camera = App->camera->fake_camera;
#endif

	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG_ENGINE("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG_ENGINE("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG_ENGINE("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG_ENGINE("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_NORMALIZE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Projection matrix for
	OnResize(App->window->width, App->window->height);

	return ret;
}

bool ModuleRenderer3D::Start()
{

	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{	
#ifdef GAME_VERSION
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearStencil(0);
	if (actual_game_camera != nullptr) {
		glClearColor(actual_game_camera->camera_color_background.r, actual_game_camera->camera_color_background.g, actual_game_camera->camera_color_background.b, actual_game_camera->camera_color_background.a);
	}
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	if (actual_game_camera != nullptr) {
		glLoadMatrixf(actual_game_camera->GetViewMatrix());
	}
#endif
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
#ifndef GAME_VERSION
	App->ui->Draw(); // last draw UI!!!
#endif

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG_ENGINE("Destroying 3D Renderer");
#ifndef GAME_VERSION
	DeleteFrameBuffers();
#endif
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
#ifndef GAME_VERSION
	glViewport(0, 0, width, height);
	App->window->width = width;
	App->window->height = height;

	CreateRenderTexture();
#else 
	glViewport(0, 0, width, height);
	App->window->width = width;
	App->window->height = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	if (actual_game_camera != nullptr) {
		glLoadMatrixf(actual_game_camera->GetProjectionMatrix());
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#endif
}

void ModuleRenderer3D::CreateRenderTexture()
{
	DeleteFrameBuffers();

	if (render_zbuffer) {
		scene_tex = new ResourceTexture();

		glGenTextures(1, &scene_tex->id);
		glBindTexture(GL_TEXTURE_2D, scene_tex->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, App->window->width, App->window->height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &z_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, z_framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, scene_tex->id, 0);
		glDepthRange(1, 0);

		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		game_tex = new ResourceTexture();

		glGenTextures(1, &game_tex->id);
		glBindTexture(GL_TEXTURE_2D, game_tex->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, App->window->width, App->window->height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &z_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, z_framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, game_tex->id, 0);
		glDepthRange(1, 0);

		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else {
		glGenFramebuffers(1, &scene_frame_buffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, scene_frame_buffer);
		glDepthRange(0, 1);
		glGenTextures(1, &scene_render_texture);
		glBindTexture(GL_TEXTURE_2D, scene_render_texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, App->window->width, App->window->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenRenderbuffers(1, &scene_depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, scene_depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, App->window->width, App->window->height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scene_render_texture, 0);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, scene_depthrenderbuffer);

		if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			LOG_ENGINE("Error creating frame buffer");
		}
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		scene_tex = new ResourceTexture("RenderTexture", scene_render_texture, App->window->width, App->window->height);

		glGenFramebuffers(1, &game_frame_buffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, game_frame_buffer);
		glDepthRange(0, 1);
		glGenTextures(1, &game_render_texture);
		glBindTexture(GL_TEXTURE_2D, game_render_texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, App->window->width, App->window->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenRenderbuffers(1, &game_depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, game_depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, App->window->width, App->window->height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, game_render_texture, 0);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, game_depthrenderbuffer);

		if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			LOG_ENGINE("Error creating frame buffer");
		}
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		game_tex = new ResourceTexture("GameTexture", game_render_texture, App->window->width, App->window->height);

		glGenFramebuffers(1, &sc_game_frame_buffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, sc_game_frame_buffer);
		glDepthRange(0, 1);
		glGenTextures(1, &sc_game_render_texture);
		glBindTexture(GL_TEXTURE_2D, sc_game_render_texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, App->window->width, App->window->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenRenderbuffers(1, &sc_game_depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, sc_game_depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, App->window->width, App->window->height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sc_game_render_texture, 0);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, sc_game_depthrenderbuffer);

		if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			LOG_ENGINE("Error creating frame buffer");
		}
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		sc_game_tex = new ResourceTexture("SelectedCameraTexture", sc_game_render_texture, App->window->width, App->window->height);
	}
}

void ModuleRenderer3D::DeleteFrameBuffers()
{
	if (scene_tex != nullptr)
	{
		delete scene_tex;
		scene_tex = nullptr;

		glDeleteFramebuffers(1, &scene_frame_buffer);
		glDeleteRenderbuffers(1, &scene_depthrenderbuffer);
		glDeleteFramebuffers(1, &z_framebuffer);
	}

	if (game_tex != nullptr)
	{
		delete game_tex;
		game_tex = nullptr;

		glDeleteFramebuffers(1, &game_frame_buffer);
		glDeleteRenderbuffers(1, &game_depthrenderbuffer);
		glDeleteFramebuffers(1, &z_framebuffer);
	}

	if (sc_game_tex != nullptr) {
		delete sc_game_tex;
		sc_game_tex = nullptr;

		glDeleteFramebuffers(1, &sc_game_frame_buffer);
		glDeleteRenderbuffers(1, &sc_game_depthrenderbuffer);
		glDeleteRenderbuffers(1, &sc_game_render_texture);
		glDeleteFramebuffers(1, &z_framebuffer);
	}
}

void ModuleRenderer3D::RenderGrid()
{
	glLineWidth(line_grid_width);
	glColor3f(grid_color.r, grid_color.g, grid_color.b);
	glBegin(GL_LINES);
	for (float i = 0; i <= length_grid; i+=grid_spacing)
	{
		glVertex3f(i - length_grid * 0.5, 0, -length_grid * 0.5);
		glVertex3f(i - length_grid * 0.5, 0, length_grid * 0.5);
		glVertex3f(-length_grid * 0.5, 0, i - length_grid * 0.5);
		glVertex3f(length_grid * 0.5, 0, i - length_grid * 0.5);
	}
	glEnd();
	glLineWidth(1);
}

void ModuleRenderer3D::ChangeDrawFrameBuffer(bool normal_frameBuffer)
{
	render_zbuffer = normal_frameBuffer;
	CreateRenderTexture();
}

void ModuleRenderer3D::UpdateCameraMatrix(ComponentCamera* camera)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf(camera->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool ModuleRenderer3D::SetCameraToDraw(const ComponentCamera * camera)
{
	if (camera == nullptr) {
		return false;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearStencil(0);
	if (App->objects->prefab_scene)
	{
		glClearColor(App->objects->prefab_color_background.r, App->objects->prefab_color_background.g, App->objects->prefab_color_background.b, App->objects->prefab_color_background.a);
	}
	else
	{
		glClearColor(camera->camera_color_background.r, camera->camera_color_background.g, camera->camera_color_background.b, camera->camera_color_background.a);
	}
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf(camera->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera->GetViewMatrix());

	return true;
}

bool ModuleRenderer3D::IsInsideFrustum(const ComponentCamera* camera, const AABB& aabb)
{
	float3 corners[8];
	aabb.GetCornerPoints(corners);

	Plane planes[6];
	camera->frustum.GetPlanes(planes);

	for (uint i = 0; i < 6; ++i)
	{
		uint point_inside_plane = 8;

		for (uint p = 0; p < 8; ++p)
		{
			if (planes[i].IsOnPositiveSide(corners[p]))
			{
				--point_inside_plane;
			}
		}

		if (point_inside_plane == 0)
		{
			return false;
		}
	}

	return true;
}
