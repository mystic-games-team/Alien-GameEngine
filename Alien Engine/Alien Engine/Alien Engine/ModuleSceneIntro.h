#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"

#include "glew/include/glew.h"
#include "par_shapes.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


public:

	

private:
	//std::vector <vec3> vertex;

	//uint id = 0;
	//int num_vertices = 2;
	//float vertices[];
	
	par_shapes_mesh* Cone1 = nullptr;
	
};
