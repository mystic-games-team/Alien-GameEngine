#include "ModuleImporter.h"
#include "glew/include/glew.h"


ModuleImporter::ModuleImporter(bool start_enabled) : Module(start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Start()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	LoadModelFile("Assets/Models/warrior.fbx");
	return true;
}

update_status ModuleImporter::Update(float dt)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	std::vector<Object3DData*>::iterator item = objects3Ddata.begin();
	for (; item != objects3Ddata.end(); ++item) {
		std::vector<Mesh*>::iterator it = (*item)->meshes.begin();

		for (; it != (*item)->meshes.end(); ++it) {

			// draw model
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f, 0.4f);

			glColor3f(0.75f, 0.75f, 0.75f);

			glBindBuffer(GL_ARRAY_BUFFER, (*it)->id_vertex);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it)->id_index);
			glVertexPointer(3, GL_FLOAT, 0, NULL);

			glDrawElements(GL_TRIANGLES, (*it)->num_index * 3, GL_UNSIGNED_INT, NULL);

			glDisable(GL_POLYGON_OFFSET_FILL);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			// draw model lines
			glColor3f(1.0f, 1.0f, 1.0f);

			glBindBuffer(GL_ARRAY_BUFFER, (*it)->id_vertex);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it)->id_index);
			glVertexPointer(3, GL_FLOAT, 0, NULL);

			glDrawElements(GL_TRIANGLES, (*it)->num_index * 3, GL_UNSIGNED_INT, NULL);

			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		}
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();

	std::vector<Object3DData*>::iterator item = objects3Ddata.begin();
	for (; item != objects3Ddata.end(); ++item) {
		if (*item != nullptr) {
			delete* item;
			*item = nullptr;
		}
	}
	objects3Ddata.clear();
	return true;
}

bool ModuleImporter::LoadModelFile(const char* path)
{
	bool ret = true;

	LOG("Loading %s", path);
	const aiScene* scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr) {
		InitScene(scene, path);
		LOG("Succesfully loaded");
	}
	else {
		ret = false;
		LOG("Error loading model %s", path);
		LOG("Error type: %s", aiGetErrorString());
	}
	aiReleaseImport(scene);
	return ret;
}

void ModuleImporter::InitScene(const aiScene* scene, const char* path)
{
	Object3DData* data = new Object3DData();
	data->path = path;
	data->textures.resize(scene->mNumMaterials);

	for (uint i = 0; i < scene->mNumMeshes; ++i) {
		const aiMesh* mesh = scene->mMeshes[i];
		data->meshes.push_back(InitMesh(mesh));
	}
	objects3Ddata.push_back(data);
}

Mesh* ModuleImporter::InitMesh(const aiMesh* ai_mesh)
{
	Mesh* mesh = new Mesh();
	mesh->material_index = ai_mesh->mMaterialIndex;

	const aiVector3D zeros(0.0f, 0.0f, 0.0f);
	mesh->vertex = new float[ai_mesh->mNumVertices * 3];
	memcpy(mesh->vertex, ai_mesh->mVertices, sizeof(float) * ai_mesh->mNumVertices * 3);
	mesh->num_vertex = ai_mesh->mNumVertices;

	if (ai_mesh->HasFaces())
	{
		mesh->num_index = ai_mesh->mNumFaces * 3;
		mesh->index = new uint[mesh->num_index]; // assume each face is a triangle
		for (uint i = 0; i < ai_mesh->mNumFaces; ++i)
		{
			if (ai_mesh->mFaces[i].mNumIndices != 3) {
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else {
				memcpy(&mesh->index[i * 3], ai_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	InitGLBuffers(mesh);

	return mesh;
}

void ModuleImporter::InitGLBuffers(Mesh* mesh)
{
	// vertex
	glGenBuffers(1, &mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 3,
		&mesh->vertex[0], GL_STATIC_DRAW);

	// index
	glGenBuffers(1, &mesh->id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index,
		&mesh->index[0], GL_STATIC_DRAW);
}

Object3DData::~Object3DData()
{
	std::vector<Mesh*>::iterator item = meshes.begin();
	for (; item != meshes.end(); ++item) {
		if (*item != nullptr) {
			delete* item;
			*item = nullptr;
		}
	}
	meshes.clear();
}
