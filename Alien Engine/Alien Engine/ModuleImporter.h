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

class ResourceModel;
class ResourceMesh;

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
	bool LoadModelFile(const char* path); // when dropped
	void LoadParShapesMesh(par_shapes_mesh* p_mesh, ComponentMesh* mesh);

	// textures
	Texture* LoadTextureFile(const char* path, bool has_been_dropped = false); // when dropped
	void ApplyTextureToSelectedObject(Texture* texture);
	
private:
	
	// models
	void InitScene(const char* path, const aiScene* scene);

	// mesh
	void LoadSceneNode(const aiNode* node, const aiScene* scene, ResourceMesh* parent);
	ResourceMesh* LoadNodeMesh(const aiScene * scene, const aiNode* node, const aiMesh* mesh, ResourceMesh* parent);
	void InitMeshBuffers(ResourceMesh* mesh);

public:

	std::vector<Texture*> textures;

private:

	ResourceModel* model = nullptr;
};
