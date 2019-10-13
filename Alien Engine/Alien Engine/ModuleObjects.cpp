#include "Globals.h"
#include "ModuleObjects.h"
#include "GameObject.h"
#include "Primitive.h"
#include "ModuleInput.h"
#include "glew/include/glew.h"
#include "Application.h"

#include "Cube.h"
#include "Sphere_Alien.h"
#include "Rock.h"
#include "Dodecahedron.h"
#include "Octahedron.h"
#include "Icosahedron.h"
#include "Torus_Alien.h"
#include "ComponentTransform.h"

ModuleObjects::ModuleObjects(bool start_enabled):Module(start_enabled)
{
	name.assign("ModuleObject");
}

ModuleObjects::~ModuleObjects()
{
}

bool ModuleObjects::Init()
{

	base_game_object = new GameObject();
	//base_game_object->AddComponent(new ComponentTransform(base_game_object, { 0,0,0 }, { 0,0,0,0 }, { 1000,1000,1000 }));

	return true;
}

bool ModuleObjects::Start()
{
	LOG("Starting Module Objects");
	bool ret = true;



	return ret;
}

update_status ModuleObjects::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleObjects::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleObjects::PostUpdate(float dt)
{

	if (allow_grid)
		App->renderer3D->RenderGrid();

	base_game_object->Draw();
	return UPDATE_CONTINUE;
}

bool ModuleObjects::CleanUp()
{

	delete base_game_object;
	base_game_object = nullptr;

	return true;
}

Primitive* ModuleObjects::CreatePrimitive(const PrimitiveType& type)
{
	Primitive* ret = nullptr;
	/*switch (type)
	{
	case PrimitiveType::CUBE:
		ret = new Cube();
		ret->type = PrimitiveType::CUBE;
		break;
	case PrimitiveType::SPHERE_ALIEN:
		ret = new Sphere_Alien();
		ret->type = PrimitiveType::SPHERE_ALIEN;
		break;
	case PrimitiveType::ROCK:
		ret = new Rock();
		ret->type = PrimitiveType::ROCK;
		break;
	case PrimitiveType::DODECAHEDRON:
		ret = new Dodecahedron();
		ret->type = PrimitiveType::DODECAHEDRON;
		break;
	case PrimitiveType::OCTAHEDRON:
		ret = new Octahedron();
		ret->type = PrimitiveType::OCTAHEDRON;
		break;
	case PrimitiveType::ICOSAHEDRON:
		ret = new Icosahedron();
		ret->type = PrimitiveType::ICOSAHEDRON;
		break;
	case PrimitiveType::TORUS:
		ret = new Torus_Alien();
		ret->type = PrimitiveType::TORUS;
		break;
	}

	if (ret != nullptr)
	{
		static_cast<GameObject*>(ret)->type = GameObjectType::PRIMITIVE;
		game_objects.push_back(ret);
	}
*/
	return ret;
}

Primitive* ModuleObjects::CreatePrimitive(const PrimitiveType& type, const float& position_x, const float& position_y, const float& position_z, const uint & subdivisions , const uint & seed , const uint & slices , const uint & slacks , const float & radius)
{
	Primitive* ret = nullptr;
	/*switch (type)
	{
	case PrimitiveType::CUBE:
		ret = new Cube(position_x, position_y, position_z);
		ret->type = PrimitiveType::CUBE;
		break;
	case PrimitiveType::SPHERE_ALIEN:
		ret = new Sphere_Alien(position_x, position_y, position_z, subdivisions);
		ret->type = PrimitiveType::SPHERE_ALIEN;
		break;
	case PrimitiveType::ROCK:
		ret = new Rock(position_x, position_y, position_z, seed, subdivisions);
		ret->type = PrimitiveType::ROCK;
		break;
	case PrimitiveType::DODECAHEDRON:
		ret = new Dodecahedron(position_x, position_y, position_z);
		ret->type = PrimitiveType::DODECAHEDRON;
		break;
	case PrimitiveType::OCTAHEDRON:
		ret = new Octahedron(position_x, position_y, position_z);
		ret->type = PrimitiveType::OCTAHEDRON;
		break;
	case PrimitiveType::ICOSAHEDRON:
		ret = new Icosahedron(position_x, position_y, position_z);
		ret->type = PrimitiveType::ICOSAHEDRON;
		break;
	case PrimitiveType::TORUS:
		ret = new Torus_Alien(position_x, position_y, position_z, slices, slacks, radius);
		ret->type = PrimitiveType::TORUS;
		break;
	}
	if (ret != nullptr)
	{
		static_cast<GameObject*>(ret)->type = GameObjectType::PRIMITIVE;
		game_objects.push_back(ret);
	}*/

	return ret;
}

void ModuleObjects::ChangeWireframeMode()
{
	view_mesh_mode = true;
	wireframe_mode = !wireframe_mode;
}

void ModuleObjects::ChangeViewMeshMode()
{
	view_mesh_mode = !view_mesh_mode;
}

void ModuleObjects::DeleteAllObjects() 
{
	std::vector<GameObject*>::iterator item = base_game_object->children.begin();
	while (item != base_game_object->children.end()) {
		if (*item != nullptr) {
			delete* item;
			*item = nullptr;
			item = base_game_object->children.erase(item);
		}
		else
			++item;
	}
	game_object_selected = nullptr;
}

void ModuleObjects::ChangeEnableGrid()
{
	allow_grid = !allow_grid;
}

void ModuleObjects::ChangeEnableNormalVertex()
{
	draw_vertex_normals = !draw_vertex_normals;
}

void ModuleObjects::ChangeEnableNormalFace()
{
	draw_face_normals = !draw_face_normals;
}

void ModuleObjects::SetNewSelectedObject(GameObject* object_selected)
{
	if (!object_selected->IsSelected()) {
		if (game_object_selected == nullptr) {
			object_selected->ChangeSelected(true);
			game_object_selected = object_selected;
		}
		else if (object_selected != game_object_selected) {
			game_object_selected->ChangeSelected(false);
			object_selected->ChangeSelected(true);
			game_object_selected = object_selected;
		}
	}
}

GameObject* ModuleObjects::GetSelectedObject()
{
	return game_object_selected;
}

void ModuleObjects::DeselectObject()
{
	game_object_selected->ChangeSelected(false);
	game_object_selected = nullptr;
}

void ModuleObjects::LoadConfig(JSONfilepack*& config) 
{
	wireframe_mode = config->GetBoolean("Configuration.Renderer.Wireframe");
	view_mesh_mode = config->GetBoolean("Configuration.Renderer.MeshView");
	draw_vertex_normals = config->GetBoolean("Configuration.Renderer.VertexNormals");
	draw_face_normals = config->GetBoolean("Configuration.Renderer.FaceNormals");
	mesh_line_width = config->GetNumber("Configuration.Renderer.MeshWidth");
	face_n_width = config->GetNumber("Configuration.Renderer.FaceNormalsWidth");
	vertex_n_width = config->GetNumber("Configuration.Renderer.VertexNormalsWidth");
	vertex_normal_length = config->GetNumber("Configuration.Renderer.VertexNormalsLength");
	face_normal_length = config->GetNumber("Configuration.Renderer.FaceNormalsLength");
	allow_grid = config->GetBoolean("Configuration.Renderer.AllowGrid");
	App->renderer3D->grid_spacing = config->GetNumber("Configuration.Renderer.GridSpacing");
	App->renderer3D->length_grid = config->GetNumber("Configuration.Renderer.GridLength");
	App->renderer3D->line_grid_width = config->GetNumber("Configuration.Renderer.GridWidth");
	App->renderer3D->grid_color.r = config->GetArrayNumber("Configuration.Renderer.GridColor", 0);
	App->renderer3D->grid_color.g = config->GetArrayNumber("Configuration.Renderer.GridColor", 1);
	App->renderer3D->grid_color.b = config->GetArrayNumber("Configuration.Renderer.GridColor", 2);
	App->renderer3D->background_color.r = config->GetArrayNumber("Configuration.Renderer.BackgroundColor", 0);
	App->renderer3D->background_color.g = config->GetArrayNumber("Configuration.Renderer.BackgroundColor", 1);
	App->renderer3D->background_color.b = config->GetArrayNumber("Configuration.Renderer.BackgroundColor", 2);
	vertex_n_color.r = config->GetArrayNumber("Configuration.Renderer.VertexNormalColor", 0);
	vertex_n_color.g = config->GetArrayNumber("Configuration.Renderer.VertexNormalColor", 1);
	vertex_n_color.b = config->GetArrayNumber("Configuration.Renderer.VertexNormalColor", 2);
	face_n_color.r = config->GetArrayNumber("Configuration.Renderer.FaceNormalColor", 0);
	face_n_color.g = config->GetArrayNumber("Configuration.Renderer.FaceNormalColor", 1);
	face_n_color.b = config->GetArrayNumber("Configuration.Renderer.FaceNormalColor", 2);
	mesh_color.r = config->GetArrayNumber("Configuration.Renderer.MeshColor", 0);
	mesh_color.g = config->GetArrayNumber("Configuration.Renderer.MeshColor", 1);
	mesh_color.b = config->GetArrayNumber("Configuration.Renderer.MeshColor", 2);
}

void ModuleObjects::SaveConfig(JSONfilepack*& config)
{
	config->SetBoolean("Configuration.Renderer.Wireframe", wireframe_mode);
	config->SetBoolean("Configuration.Renderer.MeshView", view_mesh_mode);
	config->SetBoolean("Configuration.Renderer.VertexNormals", draw_vertex_normals);
	config->SetBoolean("Configuration.Renderer.FaceNormals", draw_face_normals);
	config->SetNumber("Configuration.Renderer.MeshWidth", mesh_line_width);
	config->SetNumber("Configuration.Renderer.FaceNormalsWidth", face_n_width);
	config->SetNumber("Configuration.Renderer.VertexNormalsWidth", vertex_n_width);
	config->SetNumber("Configuration.Renderer.VertexNormalsLength", vertex_normal_length);
	config->SetNumber("Configuration.Renderer.FaceNormalsLength", face_normal_length);
	config->SetBoolean("Configuration.Renderer.AllowGrid", allow_grid);
	config->SetNumber("Configuration.Renderer.GridSpacing", App->renderer3D->grid_spacing);
	config->SetNumber("Configuration.Renderer.GridWidth", App->renderer3D->line_grid_width);
	config->SetNumber("Configuration.Renderer.GridLength", App->renderer3D->length_grid);
	config->SetArrayNumber("Configuration.Renderer.GridColor", App->renderer3D->grid_color.r);
	config->SetArrayNumber("Configuration.Renderer.GridColor", App->renderer3D->grid_color.g);
	config->SetArrayNumber("Configuration.Renderer.GridColor", App->renderer3D->grid_color.b);
	config->SetArrayNumber("Configuration.Renderer.BackgroundColor", App->renderer3D->background_color.r);
	config->SetArrayNumber("Configuration.Renderer.BackgroundColor", App->renderer3D->background_color.g);
	config->SetArrayNumber("Configuration.Renderer.BackgroundColor", App->renderer3D->background_color.b);
	config->SetArrayNumber("Configuration.Renderer.VertexNormalColor", vertex_n_color.r);
	config->SetArrayNumber("Configuration.Renderer.VertexNormalColor", vertex_n_color.g);
	config->SetArrayNumber("Configuration.Renderer.VertexNormalColor", vertex_n_color.b);
	config->SetArrayNumber("Configuration.Renderer.FaceNormalColor", face_n_color.r);
	config->SetArrayNumber("Configuration.Renderer.FaceNormalColor", face_n_color.g);
	config->SetArrayNumber("Configuration.Renderer.FaceNormalColor", face_n_color.b);
	config->SetArrayNumber("Configuration.Renderer.MeshColor", mesh_color.r);
	config->SetArrayNumber("Configuration.Renderer.MeshColor", mesh_color.g);
	config->SetArrayNumber("Configuration.Renderer.MeshColor", mesh_color.b);
}

