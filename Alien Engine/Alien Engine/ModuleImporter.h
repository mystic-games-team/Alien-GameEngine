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
#include "GameObject.h"
#include "ComponentMesh.h"
#include "Shapes.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

struct Texture {
	
	Texture(const char* path, const uint& id, const uint& height, const uint& width);
		
	std::string name;
	std::string path;

	uint id = 0;
	uint height = 0;
	uint width = 0;

};


class ModuleImporter : public Module
{
public:
	ModuleImporter(bool start_enabled = true);
	~ModuleImporter();

	bool Init();
	bool Start();
	bool CleanUp();

	bool LoadModelFile(const char* path);
	Texture* LoadTextureFile(const char* path);
	void LoadParShapesMesh(par_shapes_mesh* p_mesh, ComponentMesh* mesh);
	float3 GetObjectSize(const aiMesh* mesh);

private:

	float min_x_vertex = 0;
	float min_y_vertex = 0;
	float min_z_vertex = 0;

	float max_x_vertex=0;
	float max_y_vertex = 0;
	float max_z_vertex = 0;

	ILuint width = 0;
	ILuint height = 0;
	
private:

	void InitScene(const aiScene* scene, const char* path);
	void LoadSceneNode(const aiNode* node, const aiScene* scene, GameObject* game_object);
	GameObject* LoadNodeMesh(const aiScene * scene, const aiNode* node, const aiMesh* mesh, GameObject* game_object);
	void InitMeshBuffers(ComponentMesh* mesh);

private:

	std::vector<Texture*> textures;
	GameObject* parent_object = nullptr;
};
