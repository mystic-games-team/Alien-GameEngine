#include "Globals.h"
#include "ModuleObjects.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "glew/include/glew.h"
#include "Application.h"
#include <algorithm>
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentLight.h"
#include "ReturnZ.h"
#include "Time.h"
#include "ModuleRenderer3D.h"

ModuleObjects::ModuleObjects(bool start_enabled):Module(start_enabled)
{
	name.assign("ModuleObject");
}

ModuleObjects::~ModuleObjects()
{
	DeleteReturns();
}

bool ModuleObjects::Init()
{
	base_game_object = new GameObject();
	base_game_object->ID = 0;
	base_game_object->is_static = true;

	return true;
}

bool ModuleObjects::Start()
{
	LOG("Starting Module Objects");
	bool ret = true;

	GameObject* light_test = new GameObject(base_game_object);
	light_test->SetName("Light");
	light_test->AddComponent(new ComponentTransform(light_test, { 0,15,2.5f }, { 0,0,0,0 }, { 1,1,1 }));
	light_test->AddComponent(new ComponentLight(light_test));

	current_scene.name_without_extension = "Untitled*";
	current_scene.full_path = "Untitled*";
	current_scene.is_untitled = true;
	LoadScene("Assets/Scenes/Assigment2_scene.alienScene");

	return ret;
}

update_status ModuleObjects::PreUpdate(float dt)
{
	// delete objects
	if (need_to_delete_objects) { 
		need_to_delete_objects = false;
		base_game_object->SearchToDelete();
	}

	// change parent
	if (!to_reparent.empty()) {
		std::vector< std::tuple<GameObject*, GameObject*, bool>>::iterator item = to_reparent.begin();
		for (; item != to_reparent.end(); ++item) {
			if (std::get<0>(*item) != nullptr && std::get<1>(*item) != nullptr) {
				if (std::get<2>(*item))
					ReturnZ::AddNewAction(ReturnZ::ReturnActions::REPARENT_HIERARCHY, std::get<0>(*item));
				std::get<0>(*item)->SetNewParent(std::get<1>(*item));
			}
		}
		to_reparent.clear();
	}
	base_game_object->PreUpdate();

	return UPDATE_CONTINUE;
}

update_status ModuleObjects::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleObjects::PostUpdate(float dt)
{
	base_game_object->PostUpdate();
	if (App->renderer3D->SetCameraToDraw(App->camera->fake_camera)) {
		printing_scene = true;
		// Scene Drawing
		if (App->renderer3D->render_zbuffer) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, App->renderer3D->z_framebuffer);
		}
		else {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, App->renderer3D->scene_frame_buffer);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearStencil(0);

		if (draw_ray)
			DrawRay();

		if (allow_grid)
			App->renderer3D->RenderGrid();

		if (render_octree)
			octree.Draw();

		if (base_game_object->HasChildren()) {
			std::vector<GameObject*> to_draw;

			ComponentCamera* frustum_camera = nullptr;

			if (check_culling_in_scene)
			{
				frustum_camera = App->renderer3D->actual_game_camera;
			}
			else
			{
				frustum_camera = App->camera->fake_camera;
			}

			octree.SetStaticDrawList(&to_draw, frustum_camera);

			std::vector<GameObject*>::iterator item = base_game_object->children.begin();
			for (; item != base_game_object->children.end(); ++item) {
				if (*item != nullptr && (*item)->IsEnabled()) {
					(*item)->SetDrawList(&to_draw, frustum_camera);
				}
			}
			
			if (prefab_scene) {
				glEnable(GL_LIGHT0);
				static GLfloat f[4] = { 1,1,1,1 };
				glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, f);
			}
			item = to_draw.begin();
			for (; item != to_draw.end(); ++item) {
				if (*item != nullptr) {
					(*item)->DrawScene();
				}
			}
		}


		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	if (App->renderer3D->SetCameraToDraw(App->renderer3D->actual_game_camera)) {
		printing_scene = false;
		if (App->renderer3D->render_zbuffer) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, App->renderer3D->z_framebuffer);
		}
		else {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, App->renderer3D->game_frame_buffer);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearStencil(0);

		if (allow_grid)
			App->renderer3D->RenderGrid();

		if (base_game_object->HasChildren()) {
			std::vector<GameObject*> to_draw;

			octree.SetStaticDrawList(&to_draw, App->renderer3D->actual_game_camera);

			std::vector<GameObject*>::iterator item = base_game_object->children.begin();
			for (; item != base_game_object->children.end(); ++item) {
				if (*item != nullptr && (*item)->IsEnabled()) {
					(*item)->SetDrawList(&to_draw, App->renderer3D->actual_game_camera);
				}
			}

			item = to_draw.begin();
			for (; item != to_draw.end(); ++item) {
				if (*item != nullptr) {
					(*item)->DrawGame();
				}
			}
		}
	

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	if (App->renderer3D->selected_game_camera != nullptr && (App->objects->GetSelectedObject() != nullptr && App->renderer3D->actual_game_camera != App->objects->GetSelectedObject()->GetComponent(ComponentType::CAMERA) && App->renderer3D->SetCameraToDraw(App->renderer3D->selected_game_camera)))
	{
		printing_scene = false;
		if (App->renderer3D->render_zbuffer) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, App->renderer3D->z_framebuffer);
		}
		else {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, App->renderer3D->sc_game_frame_buffer);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearStencil(0);

		if (allow_grid)
			App->renderer3D->RenderGrid();

		if (base_game_object->HasChildren()) {
			std::vector<GameObject*> to_draw;

			octree.SetStaticDrawList(&to_draw, App->renderer3D->selected_game_camera);

			std::vector<GameObject*>::iterator item = base_game_object->children.begin();
			for (; item != base_game_object->children.end(); ++item) {
				if (*item != nullptr && (*item)->IsEnabled()) {
					(*item)->SetDrawList(&to_draw, App->renderer3D->selected_game_camera);
				}
			}

			item = to_draw.begin();
			for (; item != to_draw.end(); ++item) {
				if (*item != nullptr) {
					(*item)->DrawGame();
				}
			}
		}

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}


	return UPDATE_CONTINUE;
}

void ModuleObjects::DrawRay()
{
	if (App->camera->ray.IsFinite()) {
		glColor3f(ray_color.r, ray_color.g, ray_color.b);
		glLineWidth(ray_width);
		glBegin(GL_LINES);

		glVertex3f(App->camera->ray.a.x, App->camera->ray.a.y, App->camera->ray.a.z);
		glVertex3f(App->camera->ray.b.x, App->camera->ray.b.y, App->camera->ray.b.z);

		glEnd();
		glLineWidth(1);
	}
}

bool ModuleObjects::CleanUp()
{

	delete base_game_object;
	base_game_object = nullptr;
	// TODO: clean up returnZ both
	return true;
}

void ModuleObjects::ChangeWireframeMode()
{
	wireframe_mode = !wireframe_mode;
	base_game_object->ChangeWireframe(wireframe_mode);
}

void ModuleObjects::ChangeViewMeshMode()
{
	view_mesh_mode = !view_mesh_mode;
	base_game_object->ChangeMeshView(view_mesh_mode);
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

void ModuleObjects::ChangeEnableOutline()
{
	outline = !outline;
}


void ModuleObjects::ChangeEnableNormalVertex()
{
	draw_vertex_normals = !draw_vertex_normals;
	base_game_object->ChangeVertexNormalsView(draw_vertex_normals);
}

void ModuleObjects::ChangeEnableNormalFace()
{
	draw_face_normals = !draw_face_normals;
	base_game_object->ChangeFaceNormalsView(draw_face_normals);
}

void ModuleObjects::ChangeEnableAABB()
{
	draw_all_AABB = !draw_all_AABB;
	base_game_object->ChangeAABB(draw_all_AABB);
}

void ModuleObjects::ChangeEnableOctree()
{
	render_octree = !render_octree;
}

void ModuleObjects::ChangeEnableOBB()
{
	draw_all_OBB = !draw_all_OBB;
	base_game_object->ChangeOBB(draw_all_OBB);
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

		App->renderer3D->selected_game_camera = (ComponentCamera*)object_selected->GetComponent(ComponentType::CAMERA);
	}
}

GameObject* ModuleObjects::GetSelectedObject()
{
	return game_object_selected;
}

void ModuleObjects::DeselectObject()
{
	if (game_object_selected != nullptr) {
		if (game_object_selected->GetComponent(ComponentType::CAMERA) == App->renderer3D->selected_game_camera) {
			App->renderer3D->selected_game_camera = nullptr;
		}
		game_object_selected->ChangeSelected(false);
		game_object_selected = nullptr;
	}
}

GameObject* ModuleObjects::CreateEmptyGameObject(GameObject* parent, bool set_selected)
{
	GameObject* object = nullptr;

	if (parent != nullptr) {
		parent->open_node = true;
		object = new GameObject(parent);
		object->SetName("Empty Child");
	}
	else {
		object = new GameObject(GetRoot(false));
		object->SetName("Empty GameObject");
	}

	object->AddComponent(new ComponentTransform(object, { 0,0,0 }, { 0,0,0,0 }, { 1,1,1 }));
	
	if (set_selected)
		SetNewSelectedObject(object);

	ReturnZ::AddNewAction(ReturnZ::ReturnActions::ADD_OBJECT, object);

	return object;
}

void ModuleObjects::MoveObjectDown(GameObject* object, bool bottom)
{
	GameObject* parent = object->parent;

	if (object == parent->children.back())
		return;

	if (bottom) {
		parent->children.erase(std::find(parent->children.begin(), parent->children.end(), object));
		parent->AddChild(object);
	}
	else {
		std::vector<GameObject*>::iterator item = parent->children.begin();
		for (; item != parent->children.end(); ++item) {
			if (*item != nullptr) {
				if (*item == object) {
					item = parent->children.erase(item);
					parent->children.insert(++item, object);
					break;
				}
			}
		}
	}
}

void ModuleObjects::MoveObjectUp(GameObject* object, bool top)
{
	GameObject* parent = object->parent;

	if (object == parent->children.front())
		return;

	if (top) {
		parent->children.erase(std::find(parent->children.begin(), parent->children.end(), object));
		parent->children.insert(parent->children.begin(), object);
	}
	else {
		std::vector<GameObject*>::iterator item = parent->children.begin();
		for (; item != parent->children.end(); ++item) {
			if (*item != nullptr) {
				if (*item == object) {
					item = parent->children.erase(item);
					parent->children.insert(--item, object);
					break;
				}
			}
		}
	}
}

void ModuleObjects::MoveComponentDown(GameObject* object, Component* component, bool bottom)
{
	if (component == object->components.back())
		return;

	if (bottom) {
		object->components.erase(std::find(object->components.begin(), object->components.end(), component));
		object->components.push_back(component);
	}
	else {
		std::vector<Component*>::iterator item = object->components.begin();
		for (; item != object->components.end(); ++item) {
			if (*item != nullptr) {
				if (*item == component) {
					item = object->components.erase(item);
					object->components.insert(++item, component);
					break;
				}
			}
		}
	}
}

void ModuleObjects::MoveComponentUp(GameObject* object, Component* component, bool top)
{
	if (component == object->components.at(1))
		return;

	if (top) {
		object->components.erase(std::find(object->components.begin(), object->components.end(), component));
		object->components.insert(object->components.begin() + 1, component);
	}
	else {
		std::vector<Component*>::iterator item = object->components.begin();
		for (; item != object->components.end(); ++item) {
			if (*item != nullptr) {
				if (*item == component) {
					item = object->components.erase(item);
					object->components.insert(--item, component);
					break;
				}
			}
		}
	}
}

GameObject* ModuleObjects::GetGameObjectByID(const u64& id)
{
	return base_game_object->GetGameObjectByID(id);
}

void ModuleObjects::ReparentGameObject(GameObject* object, GameObject* next_parent, bool to_cntrlZ)
{
	if (object != nullptr && next_parent != nullptr && !object->Exists(next_parent)) {
		to_reparent.push_back({ object,next_parent,to_cntrlZ });
	}
}

void ModuleObjects::SaveScene(const char* path, bool change_scene)
{
	// remove the last save and save the new
	remove(path);

	JSON_Value* value = json_value_init_object();
	JSON_Object* object = json_value_get_object(value);
	json_serialize_to_file_pretty(value, path);

	if (value != nullptr && object != nullptr)
	{
		JSONfilepack* scene = new JSONfilepack(path, object, value);

		scene->StartSave();

		if (!base_game_object->children.empty()) { // if base game objects has children, save them
			JSONArraypack* game_objects = scene->InitNewArray("Scene.GameObjects");

			game_objects->SetAnotherNode();

			std::vector<GameObject*>::iterator item = base_game_object->children.begin();
			for (; item != base_game_object->children.end(); ++item) {
				if (*item != nullptr) {
					SaveGameObject(*item, game_objects, 1);
					if ((*item) != base_game_object->children.back())
						game_objects->SetAnotherNode();
				}
			}
		}

		scene->FinishSave();
		delete scene;
		if (change_scene) {
			std::string path_normalized = path;
			App->file_system->NormalizePath(path_normalized);
			current_scene.full_path = path;
			current_scene.is_untitled = false;
			current_scene.name_without_extension = App->file_system->GetBaseFileName(path_normalized.data());
			current_scene.need_to_save = false;
		}
	}
	else {
		LOG("Could not load scene, fail when creating the file");
	}
}

void ModuleObjects::LoadScene(const char* path, bool change_scene)
{
	JSON_Value* value = json_parse_file(path);
	JSON_Object* object = json_value_get_object(value);

	if (value != nullptr && object != nullptr)
	{
		octree.Clear();

		delete base_game_object;
		game_object_selected = nullptr;
		base_game_object = new GameObject();
		base_game_object->ID = 0;
		base_game_object->is_static = true;

		JSONfilepack* scene = new JSONfilepack(path, object, value);

		JSONArraypack* game_objects = scene->GetArray("Scene.GameObjects");

		// first is family number, second parentID, third is array index in the json file
		std::vector<std::tuple<uint, u64, uint>> objects_to_create;

		for (uint i = 0; i < game_objects->GetArraySize(); ++i) {
			uint family_number = game_objects->GetNumber("FamilyNumber");
			u64 parentID = std::stoull(game_objects->GetString("ParentID"));
			objects_to_create.push_back({ family_number,parentID, i });
			game_objects->GetAnotherNode();
		}
		std::sort(objects_to_create.begin(), objects_to_create.end(), ModuleObjects::SortByFamilyNumber);
		game_objects->GetFirstNode();
		std::vector<GameObject*> objects_created;

		std::vector<std::tuple<uint, u64, uint>>::iterator item = objects_to_create.begin();
		for (; item != objects_to_create.end(); ++item) {
			game_objects->GetNode(std::get<2>(*item));
			GameObject* obj = new GameObject();
			if (std::get<0>(*item) == 1) { // family number == 1 so parent is the base game object
				obj->LoadObject(game_objects, base_game_object);
			}
			else { // search parent
				std::vector<GameObject*>::iterator objects = objects_created.begin();
				for (; objects != objects_created.end(); ++objects) {
					if ((*objects)->ID == std::get<1>(*item)) {
						obj->LoadObject(game_objects, *objects);
						break;
					}
				}
			}
			objects_created.push_back(obj);
		}
		delete scene;
		if (change_scene) {
			std::string path_normalized = path;
			App->file_system->NormalizePath(path_normalized);
			current_scene.full_path = path;
			current_scene.is_untitled = false;
			current_scene.name_without_extension = App->file_system->GetBaseFileName(path_normalized.data());
			current_scene.need_to_save = false;
			DeleteReturns();
		}
	}
	else {
		LOG("Error loading scene %s", path);
	}
}

void ModuleObjects::CreateEmptyScene(const char* path)
{
	delete base_game_object;
	game_object_selected = nullptr;
	base_game_object = new GameObject();
	base_game_object->ID = 0;
	base_game_object->is_static = true;

	// try to remove the file because the user might have selected a name that already exists
	remove(path);

	JSON_Value* value = json_value_init_object();
	JSON_Object* object = json_value_get_object(value);
	json_serialize_to_file_pretty(value, path);

	std::string path_normalized = path;
	App->file_system->NormalizePath(path_normalized);
	current_scene.full_path = path;
	current_scene.is_untitled = false;
	current_scene.name_without_extension = App->file_system->GetBaseFileName(path_normalized.data());
	current_scene.need_to_save = false;

}

void ModuleObjects::SaveGameObject(GameObject* obj, JSONArraypack* to_save, const uint& family_number)
{
	obj->SaveObject(to_save, family_number);

	std::vector<GameObject*>::iterator item = obj->children.begin();
	for (; item != obj->children.end(); ++item) {
		if (*item != nullptr) {
			to_save->SetAnotherNode();
			SaveGameObject(*item, to_save, family_number + 1);
		}
	}
}

GameObject* ModuleObjects::GetRoot(bool ignore_prefab) 
{
	if (prefab_scene && !ignore_prefab) {
		if (base_game_object->children.size() == 0)
			return base_game_object;
		else
			return base_game_object->children.back();
	}
	else
		return base_game_object;
}

void ModuleObjects::CreateRoot()
{
	delete base_game_object;
	game_object_selected = nullptr;
	base_game_object = new GameObject();
	base_game_object->ID = 0;
	base_game_object->is_static = true;
}

void ModuleObjects::SwapReturnZ(bool get_save, bool delete_current)
{
	if (delete_current) {
		DeleteReturns();
	}

	if (get_save) {
		return_actions = save_return_actions;
		fordward_actions = save_fordward_actions;

		while (!save_return_actions.empty()) {
			save_return_actions.pop();
		}
		while (!save_fordward_actions.empty()) {
			save_fordward_actions.pop();
		}
	}
	else {
		save_return_actions = return_actions;
		save_fordward_actions = fordward_actions;

		while (!return_actions.empty()) {
			return_actions.pop();
		}
		while (!fordward_actions.empty()) {
			fordward_actions.pop();
		}
	}
}

void ModuleObjects::DeleteReturns()
{
	if (!fordward_actions.empty()) {
		while (!fordward_actions.empty()) {
			ReturnZ* act = fordward_actions.top();
			delete act;
			act = nullptr;
			fordward_actions.pop();
		}
	}
	if (!return_actions.empty()) {
		while (!return_actions.empty()) {
			ReturnZ* act = return_actions.top();
			delete act;
			act = nullptr;
			return_actions.pop();
		}
	}

}

bool ModuleObjects::SortByFamilyNumber(std::tuple<uint,u64, uint> tuple1, std::tuple<uint, u64, uint> tuple2)
{
	return std::get<0>(tuple1) < std::get<0>(tuple2);
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
	App->renderer3D->grid_color = config->GetColor("Configuration.Renderer.GridColor");
	vertex_n_color = config->GetColor("Configuration.Renderer.VertexNormalColor");
	face_n_color = config->GetColor("Configuration.Renderer.FaceNormalColor");
	mesh_color = config->GetColor("Configuration.Renderer.MeshColor");
	parent_outline_color = config->GetColor("Configuration.Renderer.ParentOutlineColor");
	no_child_outline_color = config->GetColor("Configuration.Renderer.NoChildOutlineColor");
	outline = config->GetBoolean("Configuration.Renderer.Outline");
	parent_line_width = config->GetNumber("Configuration.Renderer.ParentLineWidth");
	no_child_line_width = config->GetNumber("Configuration.Renderer.NoChildLineWidth");
	draw_all_AABB = config->GetBoolean("Configuration.Renderer.DrawAABB");
	draw_all_OBB = config->GetBoolean("Configuration.Renderer.DrawOBB");
	global_AABB_color = config->GetColor("Configuration.Renderer.ColorAABB");
	global_OBB_color = config->GetColor("Configuration.Renderer.ColorOBB");
	AABB_line_width = config->GetNumber("Configuration.Renderer.LineWidthAABB");
	OBB_line_width = config->GetNumber("Configuration.Renderer.LineWidth0BB");
	frustum_color = config->GetColor("Configuration.Renderer.FrustumColor");
	frustum_line_width = config->GetNumber("Configuration.Renderer.FrustumLineWidth");
	draw_frustum = config->GetBoolean("Configuration.Renderer.DrawFrustum");
	octree.SetBucket(config->GetNumber("Configuration.Renderer.Octreebucket"));
	octree.Recalculate(nullptr);
	octree_line_color = config->GetColor("Configuration.Renderer.OctreeColor");
	octree_line_width = config->GetNumber("Configuration.Renderer.OctreelineWidth");
	render_octree = config->GetBoolean("Configuration.Renderer.RenderOctree");
	draw_ray = config->GetBoolean("Configuration.Renderer.DrawRay");
	ray_width = config->GetNumber("Configuration.Renderer.RayWidth");
	ray_color = config->GetColor("Configuration.Renderer.RayColor");
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
	config->SetColor("Configuration.Renderer.GridColor", App->renderer3D->grid_color);
	config->SetColor("Configuration.Renderer.VertexNormalColor", vertex_n_color);
	config->SetColor("Configuration.Renderer.FaceNormalColor", face_n_color);
	config->SetColor("Configuration.Renderer.MeshColor", mesh_color);
	config->SetColor("Configuration.Renderer.ParentOutlineColor", parent_outline_color);
	config->SetColor("Configuration.Renderer.NoChildOutlineColor", no_child_outline_color);
	config->SetBoolean("Configuration.Renderer.Outline", outline);
	config->SetNumber("Configuration.Renderer.ParentLineWidth", parent_line_width);
	config->SetNumber("Configuration.Renderer.NoChildLineWidth", no_child_line_width);
	config->SetBoolean("Configuration.Renderer.DrawAABB", draw_all_AABB);
	config->SetBoolean("Configuration.Renderer.DrawOBB", draw_all_OBB);
	config->SetColor("Configuration.Renderer.ColorAABB", global_AABB_color);
	config->SetColor("Configuration.Renderer.ColorOBB", global_OBB_color);
	config->SetNumber("Configuration.Renderer.LineWidthAABB", AABB_line_width);
	config->SetNumber("Configuration.Renderer.LineWidthOBB", OBB_line_width);
	config->SetNumber("Configuration.Renderer.FrustumLineWidth", frustum_line_width);
	config->SetColor("Configuration.Renderer.FrustumColor", frustum_color);
	config->SetBoolean("Configuration.Renderer.DrawFrustum", draw_frustum);
	config->SetBoolean("Configuration.Renderer.RenderOctree", render_octree);
	config->SetNumber("Configuration.Renderer.OctreelineWidth", octree_line_width);
	config->SetColor("Configuration.Renderer.OctreeColor", octree_line_color);
	config->SetNumber("Configuration.Renderer.Octreebucket", octree.GetBucket());
	config->SetBoolean("Configuration.Renderer.DrawRay", draw_ray);
	config->SetNumber("Configuration.Renderer.RayWidth", ray_width);
	config->SetColor("Configuration.Renderer.RayColor", ray_color);
}

void ModuleObjects::CreateBasePrimitive(PrimitiveType type)
{
	GameObject* object = new GameObject(GetRoot(false));
	ComponentTransform* transform = new ComponentTransform(object, { 0,0,0 }, { 0,0,0,0 }, { 1,1,1 });
	ComponentMesh* mesh = new ComponentMesh(object);
	ComponentMaterial* material = new ComponentMaterial(object);
	
	switch (type) {
	case PrimitiveType::CUBE: {
		mesh->mesh = App->resources->GetPrimitive(PrimitiveType::CUBE);
		object->SetName("Cube");
		break; }
	case PrimitiveType::DODECAHEDRON: {
		mesh->mesh = App->resources->GetPrimitive(PrimitiveType::DODECAHEDRON);
		object->SetName("Dodecahedron");
		break; }
	case PrimitiveType::ICOSAHEDRON: {
		mesh->mesh = App->resources->GetPrimitive(PrimitiveType::ICOSAHEDRON);
		object->SetName("Icosahedron");
		break; }
	case PrimitiveType::OCTAHEDRON: {
		mesh->mesh = App->resources->GetPrimitive(PrimitiveType::OCTAHEDRON);
		object->SetName("Octahedron");
		break; }
	case PrimitiveType::ROCK: {
		mesh->mesh = App->resources->GetPrimitive(PrimitiveType::ROCK);
		object->SetName("Rock");
		break; }
	case PrimitiveType::SPHERE_ALIEN: {
		mesh->mesh = App->resources->GetPrimitive(PrimitiveType::SPHERE_ALIEN);
		object->SetName("Sphere");
		break; }
	case PrimitiveType::TORUS: {
		mesh->mesh = App->resources->GetPrimitive(PrimitiveType::TORUS);
		object->SetName("Torus");
		break; }
	default:
		break;
	}

	object->AddComponent(transform);
	object->AddComponent(mesh);
	object->AddComponent(material);
	mesh->RecalculateAABB_OBB();
	SetNewSelectedObject(object);
	ReturnZ::AddNewAction(ReturnZ::ReturnActions::ADD_OBJECT, object);
}

