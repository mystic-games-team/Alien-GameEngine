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
		if (is_scene_hovered && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			CreateRay();
		}

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			speed = camera_speed * 2 * dt;
			zoom_speed = camera_zoom_speed * 2 * dt;
		}
		if (is_scene_hovered)
		{
			Zoom();
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
			{
				Rotation(dt);
			}
		}
		if (is_scene_hovered || is_scene_focused)
		{
			Movement();
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
			Focus();
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
	float2 origin = float2(App->input->GetMousePosition().x / (float)App->ui->panel_scene->width, App->input->GetMousePosition().y / (float)App->ui->panel_scene->height);
	origin.x = (origin.x - 0.5f) * 2;
	origin.y = (origin.y - 0.5f) * 2;

	/*MousePicking(fake_camera->frustum.UnProjectLineSegment(origin.x, origin.y));*/
	LineSegment ray = fake_camera->frustum.UnProjectLineSegment(origin.x, origin.y);
	std::map<float, GameObject*> objects_hit;

	// Create a map of the objects than intersect with the ray in order by distance
	for (std::vector<GameObject*>::iterator iter = App->objects->base_game_object->children.begin(); iter != App->objects->base_game_object->children.end(); ++iter)
	{
		CreateObjectsHitMap(objects_hit, (*iter), ray);
	}

	// Check every object hit by the ray
	for (std::map<float, GameObject*>::iterator iter = objects_hit.begin(); iter != objects_hit.end(); ++iter)
	{
		ComponentMesh* mesh = (ComponentMesh*)(*iter).second->GetComponent(ComponentType::MESH);

		if (mesh != nullptr)
		{
			ResourceMesh* r_mesh = mesh->mesh;

			if (r_mesh)
			{
				LineSegment transformed_ray = ray;
				ComponentTransform* transform = (ComponentTransform*)(*iter).second->GetComponent(ComponentType::TRANSFORM);
				ray.Transform(transform->global_transformation);

				// Create every triangle in the mesh and check it versus the Ray
				for (uint check_triangles = 0; check_triangles < mesh->mesh->num_index; check_triangles += 3)
				{
					uint index_a, index_b, index_c;

					index_a = r_mesh->index[check_triangles] * 3;
					float3 point_a(&r_mesh->vertex[index_a]);

					index_b = r_mesh->index[check_triangles + 1] * 3;
					float3 point_b(&r_mesh->vertex[index_b]);

					index_c = r_mesh->index[check_triangles + 2] * 3;
					float3 point_c(&r_mesh->vertex[index_c]);

					Triangle triangle_to_check(point_a, point_b, point_c);

					if (transformed_ray.Intersects(triangle_to_check, nullptr, nullptr))
					{
						App->objects->SetNewSelectedObject((*iter).second);
						break;
					}
				}
			}
		}
	}
}

void ModuleCamera3D::CreateObjectsHitMap(std::map<float, GameObject*>& map, GameObject* go, LineSegment &ray)
{
	float distance_out = 0.f;
	float distance = 0.f;

	if (ray.Intersects(go->GetBB()) && go->children.empty())
	{
		if (ray.Intersects(go->GetGlobalOBB(),distance,distance_out))
		{
			map[distance] = go;
		}
	}

	for (std::vector<GameObject*>::iterator iter = go->children.begin(); iter != go->children.end(); ++iter)
	{
		CreateObjectsHitMap(map, go, ray);
	}
}

