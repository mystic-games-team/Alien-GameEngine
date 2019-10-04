#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "RandomHelper.h"
#include "Parson/parson.h"
#include "glew/include/glew.h"
#include "Shapes.h"

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

	//
	//vertex[0] = { 0,0,0 };
	//vertex[1] = { 0,0,1 };
	//vertex[2] = { 0,1,0 };
	//vertex[3] = { 1,0,0 };
	//vertex[4] = { 0,1,1 };
	//vertex[5] = { 1,0,1 };
	//vertex[6] = { 1,1,0 };
	//vertex[7] = { 1,1,1 };

	sphere = par_shapes_create_torus(5,10,1);
	par_shapes_translate(sphere, -2, 0, 0);



	// buffer points
	glGenBuffers(1, &my_id);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphere->npoints * 3, sphere->points, GL_STATIC_DRAW);

	// buffer index
	glGenBuffers(1, &my_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * sphere->ntriangles * 3, sphere->triangles, GL_STATIC_DRAW);


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

	par_shapes_mesh* cube = par_shapes_create_cube();


	uint my_iid = 0;
	uint my_iindex = 0;

	// buffer points
	glGenBuffers(1, &my_iid);
	glBindBuffer(GL_ARRAY_BUFFER, my_iid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cube->npoints * 3, cube->points, GL_STATIC_DRAW);

	// buffer index
	glGenBuffers(1, &my_iindex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_iindex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * cube->ntriangles * 3, cube->triangles, GL_STATIC_DRAW);


	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_iid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_iindex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, cube->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);


	glDisableClientState(GL_VERTEX_ARRAY);



	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, sphere->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);


	glDisableClientState(GL_VERTEX_ARRAY);

	return UPDATE_CONTINUE;
}
