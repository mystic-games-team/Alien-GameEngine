#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleObjects.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

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

	ilInit();
	iluInit();
	ilutInit();

	LoadModelFile("Assets/Models/cube.fbx");
	LoadTextureFile("Assets/test2.dds");

	/*GLubyte checkImage[cube_height][cube_width][4];
	for (int i = 0; i < cube_height; i++) {
		for (int j = 0; j < cube_width; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
	uint id = 0;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cube_width, cube_height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);*/

	return true;
}

update_status ModuleImporter::Update(float dt)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	std::vector<Object3DData*>::iterator item = objects3Ddata.begin();
	for (; item != objects3Ddata.end(); ++item) {
		std::vector<Mesh*>::iterator it = (*item)->meshes.begin();

		for (; it != (*item)->meshes.end(); ++it) {
			glEnable(GL_TEXTURE_2D);

			ilutRenderer(ILUT_OPENGL);
			//GLuint Texture;
			//Texture = ilutGLBindTexImage();
			glBindTexture(GL_TEXTURE_2D, test_id);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, (*it)->id_uv);
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);

			if (!App->objects->wireframe_mode) {
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
			}
			if (App->objects->wireframe_mode || App->objects->view_mesh_mode) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				// draw model lines
				glColor3f(App->objects->mesh_color.r, App->objects->mesh_color.g, App->objects->mesh_color.b);
				glLineWidth(App->objects->mesh_line_width);

				glBindBuffer(GL_ARRAY_BUFFER, (*it)->id_vertex);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it)->id_index);
				glVertexPointer(3, GL_FLOAT, 0, NULL);

				glDrawElements(GL_TRIANGLES, (*it)->num_index * 3, GL_UNSIGNED_INT, NULL);

				glLineWidth(1);
			}
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			if (App->objects->draw_vertex_normals) { // vertex normals
				glColor3f(0.0f, 1.0f, 1.0f);
				glBegin(GL_LINES);
				for (uint i = 0; i < (*it)->num_vertex * 3; i += 3)
				{
					glVertex3f((*it)->vertex[i], (*it)->vertex[i + 1], (*it)->vertex[i + 2]);
					glVertex3f((*it)->vertex[i] + (*it)->normals[i] * App->objects->vertex_normal_length, (*it)->vertex[i + 1] + (*it)->normals[i + 1] * App->objects->vertex_normal_length, (*it)->vertex[i + 2] + (*it)->normals[i + 2] * App->objects->vertex_normal_length);
				}
				glEnd();
			}
			if (App->objects->draw_face_normals) { // face normals
				glColor3f(1.0f, 0.0f, 1.0f);
				glBegin(GL_LINES);
				for (uint i = 0; i < (*it)->num_index; i += 3)
				{
					glVertex3f((*it)->center_point[i], (*it)->center_point[i + 1], (*it)->center_point[i + 2]);
					glVertex3f((*it)->center_point[i] + (*it)->center_point_normal[i] * App->objects->face_normal_length, (*it)->center_point[i + 1] + (*it)->center_point_normal[i+ 1] * App->objects->face_normal_length, (*it)->center_point[i + 2] + (*it)->center_point_normal[i + 2] * App->objects->face_normal_length);
				}
				glEnd();

			}
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

	if (scene->HasMeshes()) {
		for (uint i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* mesh = scene->mMeshes[i];
			data->meshes.push_back(InitMesh(mesh));
		}
	}
	// TODO: load .obj and other 3Ds that have textures
	/*if (scene->HasTextures()) {
		for (uint i = 0; i < scene->mNumTextures; ++i) {
			const aiMesh* mesh = scene->mMeshes[i];
			data->meshes.push_back(InitMesh(mesh));
		}
	}*/

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
	if (ai_mesh->HasNormals())
	{
		mesh->normals = new float[ai_mesh->mNumVertices * 3];
		memcpy(mesh->normals, ai_mesh->mNormals, sizeof(float) * ai_mesh->mNumVertices * 3);

		mesh->center_point_normal = new float[ai_mesh->mNumFaces * 3];
		mesh->center_point = new float[ai_mesh->mNumFaces * 3];
		mesh->num_faces = ai_mesh->mNumFaces;
		for (uint i = 0; i < mesh->num_index; i += 3)
		{
			uint index1 = mesh->index[i] * 3;
			uint index2 = mesh->index[i + 1] * 3;
			uint index3 = mesh->index[i + 2] * 3;

			vec3 x0(mesh->vertex[index1], mesh->vertex[index1 + 1], mesh->vertex[index1 + 2]);
			vec3 x1(mesh->vertex[index2], mesh->vertex[index2 + 1], mesh->vertex[index2 + 2]);
			vec3 x2(mesh->vertex[index3], mesh->vertex[index3 + 1], mesh->vertex[index3 + 2]);

			vec3 v0 = x0 - x2;
			vec3 v1 = x1 - x2;
			vec3 n = cross(v0, v1);
			
			vec3 normalized = normalize(n);

			mesh->center_point[i] = (x0.x + x1.x + x2.x) / 3;
			mesh->center_point[i + 1] = (x0.y + x1.y + x2.y) / 3;
			mesh->center_point[i + 2] = (x0.z + x1.z + x2.z) / 3;

			mesh->center_point_normal[i] = normalized.x;
			mesh->center_point_normal[i + 1] = normalized.y;
			mesh->center_point_normal[i + 2] = normalized.z;
		}
	}
	if (ai_mesh->HasTextureCoords(0)) {
		mesh->uv_cords = new float[ai_mesh->mNumVertices * 3];
		memcpy(mesh->uv_cords, (float*)ai_mesh->mTextureCoords[0], sizeof(float) * ai_mesh->mNumVertices * 3);
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

	// UV
	glGenBuffers(1, &mesh->id_uv);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_uv);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 3,
		&mesh->uv_cords[0], GL_STATIC_DRAW);

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

bool ModuleImporter::LoadTextureFile(const char* path)
{
	bool ret = true;

	


	if (ilLoadImage(path)) {
		ILuint Width, Height;
		Width = ilGetInteger(IL_IMAGE_WIDTH);
		Height = ilGetInteger(IL_IMAGE_HEIGHT);
		ILubyte* Data = ilGetData();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &test_id);
		glBindTexture(GL_TEXTURE_2D, test_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, Data);

	}



	return ret;
}