#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleObjects.h"
#include "ComponentTransform.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "PanelScene.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	name.assign("Camera3D");

}

ModuleCamera3D::~ModuleCamera3D()
{
	RELEASE(fake_camera);
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	SDL_FreeCursor(cursor);

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	frustum = &fake_camera->frustum;

	speed = camera_speed * dt;
	zoom_speed = camera_zoom_speed * dt;
	mouse_speed = camera_mouse_speed * dt;

	if (start_lerp)
	{
		fake_camera->frustum.pos = Lerp(fake_camera->frustum.pos, point_to_look, 0.5f);
		if (fake_camera->frustum.pos.Equals(point_to_look))
		{
			start_lerp = false;
		}
	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			speed = camera_speed * 2 * dt;
			zoom_speed = camera_zoom_speed * 2 * dt;
		}
		if (is_scene_hovered)
		{
			Zoom();
			if (!ImGuizmo::IsUsing() && !ImGuizmo::IsOver() && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE) {
				CreateRay();
			}

			if (!ImGuizmo::IsUsing())
				Movement();
		}
		if (!ImGuizmo::IsUsing() && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			Rotation(dt);
		}
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
			Focus();

		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			if (App->objects->GetSelectedObject() != nullptr)
			{
				fake_camera->Look(App->objects->GetSelectedObject()->GetBB().CenterPoint());
			}
		}
	}

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3& Movement)
{
	frustum->Translate(Movement);
}

// -----------------------------------------------------------------
void ModuleCamera3D::Movement()
{
	float3 movement(float3::zero);



	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT) movement += float3::unitY;
		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) movement -= float3::unitY;

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= frustum->front;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += frustum->front;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= frustum->WorldRight();
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += frustum->WorldRight();

		if (!movement.Equals(float3::zero))
		{
			frustum->Translate(movement * speed);
			reference += movement*speed;
		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		movement = float3::zero;

		cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
		SDL_SetCursor(cursor);

		if (App->input->GetMouseXMotion() > -1) movement -= frustum->WorldRight();
		if (App->input->GetMouseXMotion() < 1) movement += frustum->WorldRight();

		if (App->input->GetMouseYMotion() < 1) movement -= float3::unitY * 0.5f;
		if (App->input->GetMouseYMotion() > -1) movement += float3::unitY * 0.5f;

		if (!movement.Equals(float3::zero))
		{
			frustum->Translate(movement * mouse_speed);
			reference += movement * mouse_speed;
		}

		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_UP)
		{
			SDL_FreeCursor(cursor);
		}
	}
}

void ModuleCamera3D::Zoom()
{
	float3 zoom(float3::zero);

	if (App->input->GetMouseZ() > 0)
	{
		zoom += frustum->front;
	}
	else if (App->input->GetMouseZ() < 0)
	{
		zoom -= frustum->front;
	}

	frustum->Translate(zoom * mouse_speed);
}

void ModuleCamera3D::Rotation(float dt)
{	
	float3 distance = fake_camera->frustum.pos - reference;

	Quat rotationy(fake_camera->frustum.up, -App->input->GetMouseXMotion()* dt*0.3F);
	Quat rotationx(fake_camera->frustum.WorldRight(), -App->input->GetMouseYMotion()* dt*0.3F);

	distance = rotationx.Transform(distance);
	distance = rotationy.Transform(distance);

	fake_camera->frustum.pos = distance + reference;

	fake_camera->Look(reference);
	

	cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
	SDL_SetCursor(cursor);

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		SDL_FreeCursor(cursor);
	}
}

void ModuleCamera3D::Focus()
{
	if (App->objects->GetSelectedObject() != nullptr)
	{
		AABB bounding_box = App->objects->GetSelectedObject()->GetBB();

		if (bounding_box.IsFinite())
		{
			float offset = bounding_box.Diagonal().Length();

			fake_camera->Look(bounding_box.CenterPoint());
			reference = bounding_box.CenterPoint();
			float3 vector_distance = fake_camera->frustum.pos - bounding_box.CenterPoint();

			point_to_look = fake_camera->frustum.pos - (vector_distance - (offset * vector_distance.Normalized()));
			start_lerp = true;
		}
		else
		{
			ComponentTransform* transform = (ComponentTransform*)App->objects->GetSelectedObject()->GetComponent(ComponentType::TRANSFORM);
			float3 pos = transform->GetGlobalPosition();

			fake_camera->Look(pos);
			reference = pos;

			float3 vector_distance = fake_camera->frustum.pos - pos;
			point_to_look = fake_camera->frustum.pos - (vector_distance - (3.f * vector_distance.Normalized()));
			start_lerp = true;
		}
	}
	//else
	//{
	//	for (std::vector<GameObject*>::iterator iter = App->objects->base_game_object->children.begin(); iter != App->objects->base_game_object->children.end(); ++iter)
	//	{
	//		if (iter == App->objects->base_game_object->children.begin() || (*iter) != looking_at)
	//		{
	//			looking_at=(*iter);
	//			ComponentTransform* tr = (ComponentTransform*)(*iter)->GetComponent(ComponentType::TRANSFORM);
	//			LookAt({ tr->GetGlobalPosition().x, tr->GetGlobalPosition().y, tr->GetGlobalPosition().z });
	//		}
	//	}
	//}
}

void ModuleCamera3D::CreateRay()
{
	if (App->objects->GetRoot(true)->children.empty())
		return;

	//App->renderer3D->SetCameraToDraw(fake_camera);
	float2 origin = float2((App->input->GetMousePosition().x - App->ui->panel_scene->posX)/ App->ui->panel_scene->width, (App->input->GetMousePosition().y - App->ui->panel_scene->posY) / App->ui->panel_scene->height);

	origin.x = (origin.x - 0.5F) * 2;
	origin.y = -(origin.y - 0.5F) * 2;

	if (origin.x > 1 || origin.x < -1 || origin.y > 1 || origin.y < -1)
		return;

	ray = fake_camera->frustum.UnProjectLineSegment(origin.x, origin.y);

	std::vector<std::pair<float, GameObject*>> hits;

	// with octree to static objects
	CreateObjectsHitMap(&hits, App->objects->octree.root, ray);

	// without octree for the dynamics
	std::vector<GameObject*>::iterator item = App->objects->GetRoot(true)->children.begin();
	for (; item != App->objects->GetRoot(true)->children.end(); ++item) {
		if (*item != nullptr && (*item)->IsEnabled()) {
			CreateObjectsHitMap(&hits, (*item), ray);
		}
	}
	// sort by pos
	std::sort(hits.begin(), hits.end(), ModuleCamera3D::SortByDistance);
	static bool hit = false;
	std::vector<std::pair<float, GameObject*>>::iterator it = hits.begin();
	for (; it != hits.end(); ++it) {
		if ((*it).second != nullptr) {
			if (TestTrianglesIntersections((*it).second, ray)) {
				hit = true;
				break;
			}
		}
	}

	if (!hit) {
		App->objects->DeselectObject();
	}

	hit = false;

}

void ModuleCamera3D::CreateObjectsHitMap(std::vector<std::pair<float, GameObject*>>* hits, GameObject* go, const LineSegment &ray)
{
	float distance_out = 0.f;
	float distance = 0.f;

	if (!go->is_static) {
		if (ray.Intersects(go->GetBB(), distance, distance_out))
		{
			hits->push_back({ distance, go });
		}
	}

	for (std::vector<GameObject*>::iterator iter = go->children.begin(); iter != go->children.end(); ++iter)
	{
		if ((*iter) != nullptr && (*iter)->IsEnabled())
			CreateObjectsHitMap(hits, (*iter), ray);
	}
}

void ModuleCamera3D::CreateObjectsHitMap(std::vector<std::pair<float, GameObject*>>* hits, OctreeNode* node, const LineSegment& ray)
{
	if (node != nullptr) {

		float distance_out = 0.f;
		float distance = 0.f;

		if (ray.Intersects(node->section, distance, distance_out))
		{
			if (!node->game_objects.empty()) {
				std::vector<GameObject*>::iterator item = node->game_objects.begin();
				for (; item != node->game_objects.end(); ++item) {
					if (*item != nullptr && (*item)->IsEnabled()) {
						if (ray.Intersects((*item)->GetBB(), distance, distance_out)) {
							hits->push_back({ distance, (*item) });
						}
					}
				}
			}

			if (!node->children.empty()) {
				std::vector<OctreeNode*>::iterator item = node->children.begin();
				for (; item != node->children.end(); ++item) {
					if (*item != nullptr) {
						CreateObjectsHitMap(hits, (*item), ray);
					}
				}
			}
		}
	}
}

bool ModuleCamera3D::TestTrianglesIntersections(GameObject* object, const LineSegment& ray)
{
	bool ret = false;
	ComponentMesh* mesh = (ComponentMesh*)object->GetComponent(ComponentType::MESH);
	// TODO: if obj doesnt have mesh, just set it selected because it might be camera or light
	if (mesh != nullptr && mesh->mesh != nullptr)
	{
		ComponentTransform* transform = (ComponentTransform*)object->GetComponent(ComponentType::TRANSFORM);
		for (uint i = 0; i < mesh->mesh->num_index; i += 3)
		{
			uint index_a, index_b, index_c;

			index_a = mesh->mesh->index[i] * 3;
			float3 point_a(&mesh->mesh->vertex[index_a]);

			index_b = mesh->mesh->index[i + 1] * 3;
			float3 point_b(&mesh->mesh->vertex[index_b]);

			index_c = mesh->mesh->index[i + 2] * 3;
			float3 point_c(&mesh->mesh->vertex[index_c]);

			Triangle triangle_to_check(point_a, point_b, point_c);
			triangle_to_check.Transform(transform->global_transformation);
			if (ray.Intersects(triangle_to_check, nullptr, nullptr))
			{
				object->parent->open_node = true;
				App->objects->SetNewSelectedObject(object);
				ret = true;
				break;
			}
		}
	}
	else if (object->children.empty()){
		App->objects->SetNewSelectedObject(object);
		ret = true;
	}
	return ret;
}

bool ModuleCamera3D::SortByDistance(const std::pair<float, GameObject*> pair1, const std::pair<float, GameObject*> pair2)
{
	return pair1.first < pair2.first;
}

