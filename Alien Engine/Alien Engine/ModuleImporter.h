#pragma once

#include "Globals.h"
#include "Module.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

#include <vector>
#include "glew/include/glew.h"
#include "Model3D.h"

class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool LoadModelFile(const char* path);
	bool LoadTextureFile(const char* path);

private:

	void InitScene(const aiScene* scene, const char* path);
	void InitMesh(const aiMesh* ai_mesh, const char* path);
	void InitGLBuffers(Model3D* model3D);

	uint test_id = 0;

};
