#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleObjects.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "GameObject.h"

ModuleImporter::ModuleImporter(bool start_enabled) : Module(start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();
	iluInit();
	ilutInit();
	LOG("Initing Devil");

	return true;
}

bool ModuleImporter::Start()
{
	
	LoadTextureFile("Assets/Textures/Checkers.png");
	LoadModelFile("Assets/Models/BakerHouse.fbx");

	ReadModelMetaData("Library/Models/BakerHouse.alien");

	return true;
}

bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();

	std::vector<Texture*>::iterator item = textures.begin();
	for (; item != textures.end(); ++item) {
		if (*item != nullptr) {
			delete* item;
			*item = nullptr;
		}
	}
	textures.clear();
	
	return true;
}

bool ModuleImporter::LoadModelFile(const char* path)
{
	bool ret = true;

	LOG("Loading %s", path);

	// check if this file has been already created as a .alien
	if (!App->file_system->Exists((std::string(LIBRARY_MODELS_FOLDER) + App->file_system->GetBaseFileName(path) + std::string(".alienModel")).data())) {
		
		const aiScene* scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_MaxQuality);

		if (scene != nullptr) {
			CreateModelMetaData(path, scene);
		}
		else {
			ret = false;
			LOG("Error loading model %s", path);
			LOG("Error type: %s", aiGetErrorString());
		}

		aiReleaseImport(scene);

		/*if (scene != nullptr) {
			InitScene(scene, path);
			LOG("Succesfully loaded %s", path);
		}
		else {
			ret = false;
			LOG("Error loading model %s", path);
			LOG("Error type: %s", aiGetErrorString());
		}
		aiReleaseImport(scene);*/

	}
	else {
		// Load the .alienModel directly!!
	}


	
	
	return ret;
}

void ModuleImporter::InitScene(const aiScene* scene, const char* path)
{
	if (scene->mRootNode->mNumChildren > 1) { // if there is more than one children we create an empty game object that is going ot be the parent of every child
		// create the parent of the all fbx/obj...
		parent_object = new GameObject(App->objects->base_game_object);
		ComponentTransform* tr = new ComponentTransform(parent_object, { 0,0,0 }, { 0,0,0,0 }, { 1,1,1 });
		parent_object->AddComponent(tr);
		// set parent name, we must change that
		parent_object->SetName(App->file_system->GetBaseFileName(path).data());
		// start recursive function to pass through all nodes
		LoadSceneNode(scene->mRootNode, scene, parent_object);
		// set parent active
		App->objects->SetNewSelectedObject(parent_object);
		LOG("All nodes loaded");
		parent_object = nullptr;
	}
	else { // if there is just one child, we dont need an empty game object. So the parent of this child is de base_game_object
		LoadSceneNode(scene->mRootNode, scene, App->objects->base_game_object);
	}
	
}

void ModuleImporter::LoadSceneNode(const aiNode* node, const aiScene* scene, GameObject* parent)
{
	LOG("Loading node with name %s", node->mName.C_Str());
	GameObject* next_parent = nullptr;
	if (parent == App->objects->base_game_object)
		next_parent = App->objects->base_game_object;

	for (uint i = 0; i < node->mNumMeshes; ++i) {
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		next_parent = LoadNodeMesh(scene, node, mesh, parent);
	}
	
	for (uint i = 0; i < node->mNumChildren; ++i) {
		LOG("Loading children of node %s", node->mName.C_Str());
		LoadSceneNode(node->mChildren[i], scene, next_parent);
	}
}

GameObject* ModuleImporter::LoadNodeMesh(const aiScene * scene, const aiNode* node, const aiMesh* ai_mesh, GameObject* parent)
{
	GameObject* ret = nullptr;
	if (parent == nullptr) {
		ret = new GameObject(parent_object);
	}
	else {
		ret = new GameObject(parent);
	}

	// get mesh data
	ComponentMesh* mesh = new ComponentMesh(ret);
	// get vertex
	mesh->vertex = new float[ai_mesh->mNumVertices * 3];
	
	memcpy(mesh->vertex, ai_mesh->mVertices, sizeof(float) * ai_mesh->mNumVertices * 3);
	mesh->num_vertex = ai_mesh->mNumVertices;
	// get index
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
	// get normals
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
	// get UV
	if (ai_mesh->HasTextureCoords(0)) {
		mesh->uv_cords = new float[ai_mesh->mNumVertices * 3];
		memcpy(mesh->uv_cords, (float*)ai_mesh->mTextureCoords[0], sizeof(float) * ai_mesh->mNumVertices * 3);
	}

	// aabb
	mesh->aabb_min.x = ai_mesh->mAABB.mMin.x;
	mesh->aabb_min.y = ai_mesh->mAABB.mMin.y;
	mesh->aabb_min.z = ai_mesh->mAABB.mMin.z;

	mesh->aabb_max.x = ai_mesh->mAABB.mMax.x;
	mesh->aabb_max.y = ai_mesh->mAABB.mMax.y;
	mesh->aabb_max.z = ai_mesh->mAABB.mMax.z;

	// set the material
	ComponentMaterial* material = new ComponentMaterial(ret);
	material->material_index = ai_mesh->mMaterialIndex;
	aiMaterial* ai_material = scene->mMaterials[material->material_index];
	uint numTextures = ai_material->GetTextureCount(aiTextureType_DIFFUSE);
	aiString path;
	ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
	std::string full_path(TEXTURES_FOLDER + std::string(path.C_Str()));
	material->texture = LoadTextureFile(full_path.data());

	InitMeshBuffers(mesh);

	// get local transformations
	aiVector3D translation, scaling;
	aiQuaternion rotation;
	// local pos, rot & scale
	node->mTransformation.Decompose(scaling, rotation, translation);

	// set the scale in value of 1 but keeping the dimensions
	int max_ = max(scaling.x, scaling.y);
	max_ = max(max_, scaling.z);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x / max_, scaling.y / max_, scaling.z / max_);
	//float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	ComponentTransform* transform = new ComponentTransform(ret, pos, rot, scale);

	ret->AddComponent(transform);
	ret->AddComponent(mesh);
	ret->AddComponent(material);
	ret->SetName(node->mName.C_Str());

	return ret;
}

void ModuleImporter::InitMeshBuffers(ComponentMesh* mesh)
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

	if (mesh->uv_cords != nullptr) {
		// UV
		glGenBuffers(1, &mesh->id_uv);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_uv);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 3,
			&mesh->uv_cords[0], GL_STATIC_DRAW);
	}

	if (mesh->normals != nullptr) {
		// normals
		glGenBuffers(1, &mesh->id_normals);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_normals);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 3,
			&mesh->normals[0], GL_STATIC_DRAW);
	}
	
}

Texture* ModuleImporter::LoadTextureFile(const char* path, bool has_been_dropped)
{
	Texture* texture = nullptr;

	std::vector<Texture*>::iterator item = textures.begin();
	for (; item != textures.end(); ++item) {
		if (*item != nullptr && (*item)->path == path) {
			if (has_been_dropped && App->objects->GetSelectedObject() != nullptr) {
				ApplyTextureToSelectedObject(*item);
			}
			return (*item);
		}
	}

	ILuint new_image_id = 0;
	ilGenImages(1, &new_image_id);
	ilBindImage(new_image_id);

	ilutRenderer(ILUT_OPENGL);  

	if (ilLoadImage(path)) {
		iluFlipImage();
		texture = new Texture(path, ilutGLBindTexImage(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, texture->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);
		textures.push_back(texture);

		if (has_been_dropped && App->objects->GetSelectedObject() != nullptr) {
			ApplyTextureToSelectedObject(texture);
		}
	}
	else {
		LOG("Error while loading image in %s", path);
		LOG("Error: %s", ilGetString(ilGetError()));
	}

	ilDeleteImages(1, &new_image_id);

	return texture;
}

void ModuleImporter::ApplyTextureToSelectedObject(Texture* texture)
{
	GameObject* selected = App->objects->GetSelectedObject();
	ComponentMaterial* material = (ComponentMaterial*)selected->GetComponent(ComponentType::MATERIAL);
	if (selected->HasComponent(ComponentType::MESH)) {
		if (material == nullptr) {
			material = new ComponentMaterial(selected);
			selected->AddComponent(material);
		}
		material->texture = texture;
	}
}

void ModuleImporter::CreateModelMetaData(const char* path, const aiScene* scene)
{
	uint num_meshes = scene->mNumMeshes;

	// file size
	uint size = sizeof(num_meshes) + sizeof(char*) * num_meshes;

	char* data = new char[size]; // Allocate
	char* cursor = data;

	// save the number of meshes
	uint bytes = sizeof(num_meshes); 

	memcpy(cursor, &num_meshes, bytes);
	cursor += bytes;


	for (uint i = 0; i < scene->mRootNode->mNumChildren; ++i) {
		if (scene->mRootNode->mChildren[i] != nullptr) {
			CreateNodeMetaData(scene, scene->mRootNode->mChildren[i], &cursor);
		}
	}
	
	// Create the file
	std::string output;
	App->file_system->SaveUnique(output, data, size, LIBRARY_MODELS_FOLDER, App->file_system->GetBaseFileName(path).data(), ".alien");
	LOG("Created file %s", output.data());

	RELEASE_ARRAY(data); 
}

void ModuleImporter::CreateNodeMetaData(const aiScene* scene, const aiNode* node, char** cursor)
{
	for (uint i = 0; i < node->mNumMeshes; ++i) {
		const aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];
		if (ai_mesh != nullptr) {
			// copy the path to the .alien file
			char* node_path[1] = { CreateMeshMetaData(node, ai_mesh) };
			memcpy(*cursor, node_path, sizeof(node_path));
			*cursor += sizeof(node_path);
		}
	}

	// if it has children, call again the function with them
	if (node->mNumChildren > 0) { 
		for (uint i = 0; i < node->mNumChildren; ++i) {
			if (node->mChildren[i] != nullptr) {
				CreateNodeMetaData(scene, node->mChildren[i], cursor);
			}
		}
	}
}

char* ModuleImporter::CreateMeshMetaData(const aiNode* node, const aiMesh* ai_mesh)
{

	// per saber la jerarquia fer que cada mesh es guardi el seu node name i el parent node name

	return nullptr;
}

void ModuleImporter::ReadModelMetaData(const char* path)
{
	// get the file buffer
	char* data = nullptr;
	App->file_system->Load(path, &data);

	if (data != nullptr) {
		uint num_meshes = 0;

		uint bytes = sizeof(num_meshes);
		memcpy(&num_meshes, data, bytes);
		data += bytes;

		for (uint i = 0; i < num_meshes; ++i) {

			// get the name of the nodes path
			char* name[1];
			bytes = sizeof(name);
			memcpy(name, data, bytes);
			data += bytes;

			// read the mesh meta data
			ReadMeshMetaData(name[0]);
		}
		RELEASE_ARRAY(data);
	}
}

void ModuleImporter::ReadMeshMetaData(const char* path)
{




}

void ModuleImporter::LoadParShapesMesh(par_shapes_mesh* shape, ComponentMesh* mesh)
{
	par_shapes_unweld(shape, true);
	par_shapes_compute_normals(shape);

	mesh->num_vertex = shape->npoints;
	mesh->num_index = shape->ntriangles * 3;

	mesh->vertex = new float[mesh->num_vertex * 3];
	mesh->index = new uint[mesh->num_index * 3];

	memcpy(mesh->vertex, shape->points, sizeof(float) * mesh->num_vertex * 3);
	memcpy(mesh->index, shape->triangles, sizeof(PAR_SHAPES_T) * mesh->num_index);

	
	if (shape->tcoords != nullptr) {
		mesh->uv_cords = new float[mesh->num_vertex * 3];
		memcpy(mesh->uv_cords, shape->tcoords, sizeof(float) * mesh->num_vertex * 3);
	}

	if (shape->normals != nullptr) {
		mesh->normals = new float[mesh->num_vertex * 3];

		memcpy(mesh->normals, shape->normals, sizeof(float) * mesh->num_vertex * 3);

		mesh->center_point_normal = new float[shape->ntriangles * 3];
		mesh->center_point = new float[shape->ntriangles * 3];
		mesh->num_faces = shape->ntriangles;
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

	InitMeshBuffers(mesh);
}

Texture::Texture(const char* path, const uint& id, const uint & width, const int & height)
{
	this->id = id;
	this->height = height;
	this->width = width;
	this->path = std::string(path);
	name = App->file_system->GetBaseFileName(path);
}


