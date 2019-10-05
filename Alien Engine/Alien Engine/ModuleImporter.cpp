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
	LoadModelFile("Assets/warrior.fbx");
	return true;
}

update_status ModuleImporter::Update(float dt)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	std::vector<Object3DData*>::iterator item = fbx_data.begin();
	for (; item != fbx_data.end(); ++item) {
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

			glDrawElements(GL_TRIANGLES, (*it)->num_index, GL_UNSIGNED_INT, NULL);

			glDisable(GL_POLYGON_OFFSET_FILL);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			// draw model lines
			glColor3f(1.0f, 1.0f, 1.0f);

			glBindBuffer(GL_ARRAY_BUFFER, (*it)->id_vertex);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it)->id_index);
			glVertexPointer(3, GL_FLOAT, 0, NULL);

			glDrawElements(GL_TRIANGLES, (*it)->num_index, GL_UNSIGNED_INT, NULL);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		}
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

bool ModuleImporter::LoadModelFile(const char* path)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr) {
		InitScene(scene, path);
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
	fbx_data.push_back(data);
}

Mesh* ModuleImporter::InitMesh(const aiMesh* ai_mesh)
{
	Mesh* mesh = new Mesh();
	mesh->material_index = ai_mesh->mMaterialIndex;

	const aiVector3D zeros(0.0f, 0.0f, 0.0f);

	// save vertex, normals and text cord
	for (uint i = 0; i < ai_mesh->mNumVertices; ++i) { 
		const aiVector3D* vertex_pos = &(ai_mesh->mVertices[i]);
		const aiVector3D* normal_pos = &(ai_mesh->mNormals[i]);
		const aiVector3D* text_cords = ai_mesh->HasTextureCoords(0) ? &(ai_mesh->mTextureCoords[0][i]) : &zeros;

		// vertex
		mesh->vertex.push_back(vertex_pos->x);
		mesh->vertex.push_back(vertex_pos->y);
		mesh->vertex.push_back(vertex_pos->z);
		// normals
		mesh->normals.push_back(normal_pos->x);
		mesh->normals.push_back(normal_pos->y);
		mesh->normals.push_back(normal_pos->z);
		// texture cords
		mesh->texture_cords.push_back(text_cords->x);
		mesh->texture_cords.push_back(text_cords->y);
		mesh->texture_cords.push_back(text_cords->z);
	}
	mesh->num_vertex = mesh->vertex.size();

	for (uint i = 0; i < ai_mesh->mNumFaces; ++i) {
		const aiFace& face = ai_mesh->mFaces[i];
		if (face.mNumIndices < 3) {
			LOG("Face with less than 3 index");
		}
		else {
			mesh->index.push_back(face.mIndices[0]);
			mesh->index.push_back(face.mIndices[1]);
			mesh->index.push_back(face.mIndices[2]);
		}
	}
	mesh->num_index = mesh->index.size();

	InitGLBuffers(mesh);

	return mesh;
}

void ModuleImporter::InitGLBuffers(Mesh* mesh)
{
	// vertex
	glGenBuffers(1, &mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex,
		&mesh->vertex[0], GL_STATIC_DRAW);

	// index
	glGenBuffers(1, &mesh->id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index,
		&mesh->index[0], GL_STATIC_DRAW);
}
