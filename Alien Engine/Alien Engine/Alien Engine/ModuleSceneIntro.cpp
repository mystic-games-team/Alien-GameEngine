#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "RandomHelper.h"
#include "Parson/parson.h"
#include "glew/include/glew.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	name.assign("SceneIntro");
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	//Sphere s({ 0,0,0 }, 4);
	//Sphere f({ 0,0,0 }, 7);
	//if (f.Intersects(s)) {
	//	LOG("INTERSECTION DETECTED!!!");
	//}
	//
	//LOG("RAND %.1f",GetRandomFloatBetweenTwo(-103.3f,1.8f));
	//
	//glBegin(GL_TRIANGLES);

	//// Face 1
	//glVertex3f(0, 0, 0);
	//glVertex3f(1, 0, 0);
	//glVertex3f(0, 1, 0);
	//
	//glVertex3f(1, 0, 0);
	//glVertex3f(1, 1, 0);
	//glVertex3f(0, 1, 0);

	//// Face 2
	//glVertex3f(0, 1, 0);
	//glVertex3f(1, 1, 0);
	//glVertex3f(0, 1, -1);
	//
	//glVertex3f(0, 1, -1);
	//glVertex3f(1, 1, 0);
	//glVertex3f(1, 1, -1);

	//// Face 3
	//glVertex3f(1, 1, 0);
	//glVertex3f(1, 0, 0);
	//glVertex3f(1, 0, -1);

	//glVertex3f(1, 1, 0);
	//glVertex3f(1, 0, -1);
	//glVertex3f(1, 1, -1);

	//// Face 4
	//glVertex3f(0, 1, 0);
	//glVertex3f(0, 1, -1);
	//glVertex3f(0, 0, 0);

	//glVertex3f(0, 1, -1);
	//glVertex3f(0, 0, -1);
	//glVertex3f(0, 0, 0);

	//// Face 5
	//glVertex3f(0, 1, -1);
	//glVertex3f(1, 1, -1);
	//glVertex3f(1, 0, -1);

	//glVertex3f(0, 1, -1);
	//glVertex3f(1, 0, -1);
	//glVertex3f(0, 0, -1);

	//// Face 6
	//glVertex3f(0, 0, 0);
	//glVertex3f(0, 0, -1);
	//glVertex3f(1, 0, 0);

	//glVertex3f(0, 0, -1);
	//glVertex3f(1, 0, -1);
	//glVertex3f(1, 0, 0);

	//glEnd();

	vertices[0] = 0;
	vertices[1] = 0;
	vertices[2] = 0;
	vertices[3] = 0;
	vertices[4] = 0;
	vertices[5] = 1;

	glEnableClientState(GL_VERTEX_ARRAY);

	glGenBuffers(1, (GLuint*)& id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, vertices, GL_STATIC_DRAW);


	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … draw other buffers
	glDrawArrays(GL_LINES, 0, num_vertices);
	glDisableClientState(GL_VERTEX_ARRAY);

	return UPDATE_CONTINUE;
}


