#pragma once

#include "Globals.h"
#include "Module.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#pragma comment (lib, "Devil/lib/x86/DevIL.lib")
#pragma comment (lib, "Devil/lib/x86/ILU.lib")
#pragma comment (lib, "Devil/lib/x86/ILUT.lib")

#include <vector>
#include "glew/include/glew.h"

struct Mesh {

	~Mesh() {

		glDeleteBuffers(num_vertex, &id_vertex);
		glDeleteBuffers(num_index, &id_index);

		delete[] index;
		delete[] vertex;
		delete[] normals;
		delete[] uv_cords;
		delete[] center_point_normal;
		delete[] center_point;

		center_point_normal = nullptr;
		center_point = nullptr;
		index = nullptr;
		vertex = nullptr;
		normals = nullptr;
		uv_cords = nullptr;
	}

	uint id_index = 0;
	uint id_vertex = 0; 

	uint num_index = 0; 
	uint* index = nullptr; 

	uint num_vertex = 0; 
	float* vertex = nullptr; 

	uint material_index = 0; 

	float* normals = nullptr;

	float* center_point_normal = nullptr;
	float* center_point = nullptr;
	uint num_faces = 0;

	float* uv_cords = nullptr; 
	uint id_uv = 0;
};

struct Textures {

};

struct Object3DData {

	~Object3DData();

	std::vector<Mesh*> meshes;
	std::vector<Textures*> textures; // TODO clean up this :)
	const char* path = nullptr;
};

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
	Mesh* InitMesh(const aiMesh* ai_mesh);
	void InitGLBuffers(Mesh* mesh);

	uint ImageName;
	uint test_id = 0;

private:

	std::vector<Object3DData*> objects3Ddata;

};
