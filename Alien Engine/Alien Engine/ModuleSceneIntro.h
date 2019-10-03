#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"

#include "glew/include/glew.h"
#include "Shapes.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


public:
	par_shapes_mesh* cube = nullptr;
	par_shapes_mesh* sphere = nullptr;

	uint cube_id = 0;
	uint sphere_id = 0;
	uint cube_index = 0;
	uint sphere_index = 0;
};
