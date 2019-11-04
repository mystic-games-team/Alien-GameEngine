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
#include "ModuleCamera3D.h"

#include "ModuleResources.h"
#include "ResourceMesh.h"
#include "ResourceModel.h"
#include "ResourceTexture.h"


ModuleImporter::ModuleImporter(bool start_enabled) : Module(start_enabled)
{
	name = "Importer";
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
	//LoadModelFile("Assets/Models/BakerHouse.fbx");

	return true;
}

bool ModuleImporter::CleanUp()
{
	aiDetachAllLogStreams();
	
	return true;
}

bool ModuleImporter::LoadModelFile(const char* path)
{
	bool ret = true;

	LOG("Loading %s", path);

	std::string meta_path = std::string((LIBRARY_MODELS_FOLDER) + App->file_system->GetBaseFileName(path) + std::string(".alien")).data();

	// if this file has been already imported just load the .alien
	if (!App->file_system->Exists(meta_path.data())) {
		
		const aiScene* scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_GenBoundingBoxes);

		if (scene != nullptr) {
			InitScene(path, scene);
			LOG("Succesfully loaded %s", path);
		}
		else {
			ret = false;
			LOG("Error loading model %s", path);
			LOG("Error type: %s", aiGetErrorString());
		}
		aiReleaseImport(scene);
		App->resources->AddNewFileNode(path, true);
	}
	else {
		App->resources->CreateNewModelInstanceOf(meta_path.data());
	}
	
	return ret;
}

void ModuleImporter::InitScene(const char* path, const aiScene* scene)
{
	model = new ResourceModel();
	model->name = App->file_system->GetBaseFileName(path);
	model->path = std::string(path);

	// start recursive function to all nodes
	LoadSceneNode(scene->mRootNode, scene, nullptr, 1);

	// create the meta data files like .alien
	model->CreateMetaData(); // TODO: CreateMetaData of: transformations & texture path

	// TODO: fer que es faci pushback nomes si tot ha sortit be

	App->resources->AddResource(model);
	model = nullptr;
}


void ModuleImporter::LoadSceneNode(const aiNode* node, const aiScene* scene, ResourceMesh* parent, uint family_number)
{
	LOG("Loading node with name %s", node->mName.C_Str());
	ResourceMesh* next_parent = nullptr;

	for (uint i = 0; i < node->mNumMeshes; ++i) {
		const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		next_parent = LoadNodeMesh(scene, node, mesh, parent);
		next_parent->family_number = family_number;
		App->resources->AddResource(next_parent);
		model->meshes_attached.push_back(next_parent);
	}
	
	for (uint i = 0; i < node->mNumChildren; ++i) {
		LOG("Loading children of node %s", node->mName.C_Str());
		uint fam_num = 1;
		if (next_parent != nullptr)
			fam_num = next_parent->family_number + 1;
		LoadSceneNode(node->mChildren[i], scene, next_parent, fam_num);
	}
}

ResourceMesh* ModuleImporter::LoadNodeMesh(const aiScene * scene, const aiNode* node, const aiMesh* ai_mesh, ResourceMesh* parent)
{
	// TODO: resource mesh should do this??

	ResourceMesh* ret = new ResourceMesh();
	if (parent != nullptr)
		ret->parent_name = parent->name;

	// get vertex
	ret->vertex = new float[ai_mesh->mNumVertices * 3];
	memcpy(ret->vertex, ai_mesh->mVertices, sizeof(float) * ai_mesh->mNumVertices * 3);
	ret->num_vertex = ai_mesh->mNumVertices;

	// get index
	if (ai_mesh->HasFaces())
	{
		ret->num_index = ai_mesh->mNumFaces * 3;
		ret->index = new uint[ret->num_index]; // assume each face is a triangle
		for (uint i = 0; i < ai_mesh->mNumFaces; ++i)
		{
			if (ai_mesh->mFaces[i].mNumIndices != 3) {
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else {
				memcpy(&ret->index[i * 3], ai_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}
	// get normals
	if (ai_mesh->HasNormals())
	{
		ret->normals = new float[ai_mesh->mNumVertices * 3];
		memcpy(ret->normals, ai_mesh->mNormals, sizeof(float) * ai_mesh->mNumVertices * 3);

		ret->center_point_normal = new float[ai_mesh->mNumFaces * 3];
		ret->center_point = new float[ai_mesh->mNumFaces * 3];
		ret->num_faces = ai_mesh->mNumFaces;
		for (uint i = 0; i < ret->num_index; i += 3)
		{
			uint index1 = ret->index[i] * 3;
			uint index2 = ret->index[i + 1] * 3;
			uint index3 = ret->index[i + 2] * 3;

			vec3 x0(ret->vertex[index1], ret->vertex[index1 + 1], ret->vertex[index1 + 2]);
			vec3 x1(ret->vertex[index2], ret->vertex[index2 + 1], ret->vertex[index2 + 2]);
			vec3 x2(ret->vertex[index3], ret->vertex[index3 + 1], ret->vertex[index3 + 2]);

			vec3 v0 = x0 - x2;
			vec3 v1 = x1 - x2;
			vec3 n = cross(v0, v1);

			vec3 normalized = normalize(n);

			ret->center_point[i] = (x0.x + x1.x + x2.x) / 3;
			ret->center_point[i + 1] = (x0.y + x1.y + x2.y) / 3;
			ret->center_point[i + 2] = (x0.z + x1.z + x2.z) / 3;

			ret->center_point_normal[i] = normalized.x;
			ret->center_point_normal[i + 1] = normalized.y;
			ret->center_point_normal[i + 2] = normalized.z;
		}
	}
	// get UV
	if (ai_mesh->HasTextureCoords(0)) {
		ret->uv_cords = new float[ai_mesh->mNumVertices * 3];
		memcpy(ret->uv_cords, (float*)ai_mesh->mTextureCoords[0], sizeof(float) * ai_mesh->mNumVertices * 3);
	}

	ret->InitBuffers();

	// set the material
	aiMaterial* ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
	aiString path;
	ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
	std::string full_path(TEXTURES_FOLDER + std::string(path.C_Str()));
	ret->texture = LoadTextureFile(full_path.data());

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

	ret->pos = pos;
	ret->scale = scale;
	ret->rot = rot;
	ret->name = std::string(node->mName.C_Str());

	return ret;
}

ResourceTexture* ModuleImporter::LoadTextureFile(const char* path, bool has_been_dropped, bool is_custom)
{
	ResourceTexture* texture = nullptr;

	std::vector<ResourceTexture*>::iterator item = App->resources->resource_textures.begin();
	for (; item != App->resources->resource_textures.end(); ++item) {
		if (*item != nullptr && (*item)->GetAssetsPath() == path) {
			if (has_been_dropped && App->objects->GetSelectedObject() != nullptr) {
				ApplyTextureToSelectedObject(*item);
			}
			LOG("This texture was already loaded");
			return (*item);
		}
	}

	ILuint new_image_id = 0;
	ilGenImages(1, &new_image_id);
	ilBindImage(new_image_id);

	ilutRenderer(ILUT_OPENGL);  

	if (ilLoadImage(path)) {
		iluFlipImage();
		texture = new ResourceTexture(path, ilutGLBindTexImage(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
		texture->is_custom = is_custom;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, texture->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);
		
		App->resources->AddResource(texture);



		if (has_been_dropped && App->objects->GetSelectedObject() != nullptr) {
			ApplyTextureToSelectedObject(texture);
		}

		if (is_custom)
			App->resources->AddNewFileNode(path, true);

		LOG("Texture successfully loaded: %s", path);
	}
	else {
		LOG("Error while loading image in %s", path);
		LOG("Error: %s", ilGetString(ilGetError()));
	}

	ilDeleteImages(1, &new_image_id);

	return texture;
}

void ModuleImporter::ApplyTextureToSelectedObject(ResourceTexture* texture)
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

void ModuleImporter::LoadParShapesMesh(par_shapes_mesh* shape, ComponentMesh* mesh)
{
	/*par_shapes_unweld(shape, true);
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
	}*/

	//InitMeshBuffers(mesh);
}



