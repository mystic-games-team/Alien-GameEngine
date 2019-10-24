#pragma once

#include "Globals.h"
#include "Module.h"


#include "Assimp/include/assimp/cimport.h"
#include "Assimp/include/assimp/scene.h"
#include "Assimp/include/assimp/postprocess.h"
#include "Assimp/include/assimp/cfileio.h"
#include "Assimp/include/assimp/mesh.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

#include <vector>
#include "glew/include/glew.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "Shapes.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

struct Texture {
	
	Texture(const char* path, const uint& id, const uint & width, const int & height);
		
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

	// models
	bool LoadModelFile(const char* path);
	void LoadParShapesMesh(par_shapes_mesh* p_mesh, ComponentMesh* mesh);

	// textures
	Texture* LoadTextureFile(const char* path, bool has_been_dropped = false);
	void ApplyTextureToSelectedObject(Texture* texture);
	
private:
	
	// models
	void CreateModelMetaData(const char* path, const aiScene* scene); // create the .alien 
	void CreateNodeMetaData(const aiNode* node, char** cursor); // create the .alienMesh 

	void ReadModelMetaData(const char* path);
	void ReadMeshMetaData(const char* path);

	// mesh
	void InitScene(const aiScene* scene, const char* path);
	void LoadSceneNode(const aiNode* node, const aiScene* scene, GameObject* game_object);
	GameObject* LoadNodeMesh(const aiScene * scene, const aiNode* node, const aiMesh* mesh, GameObject* game_object);
	void InitMeshBuffers(ComponentMesh* mesh);

public:

	std::vector<Texture*> textures;

private:

	GameObject* parent_object = nullptr;
};
