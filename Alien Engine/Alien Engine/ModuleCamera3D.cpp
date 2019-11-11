#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleObjects.h"
#include "ComponentTransform.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"

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
			Rotation();
		}
	}
	if (is_scene_hovered || is_scene_focused) 
	{
		Movement();
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		Focus();

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

void ModuleCamera3D::Rotation()
{
	float3 point = (float3::zero);

	if (App->objects->GetSelectedObject() != nullptr)
	{
		ComponentTransform* transform = (ComponentTransform*)App->objects->GetSelectedObject()->GetComponent(ComponentType::TRANSFORM);
		point = transform->GetGlobalPosition();
	}

	float3 focus = fake_camera->frustum.pos - point;

	Quat rotationy(fake_camera->frustum.up, App->input->GetMouseXMotion()*0.1f);
	Quat rotationx(fake_camera->frustum.WorldRight(), App->input->GetMouseYMotion()*0.05f);

	focus = rotationx.Transform(focus);
	focus = rotationy.Transform(focus);

	fake_camera->frustum.pos = focus + point;

	fake_camera->Look(point);

	cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
	SDL_SetCursor(cursor);

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		SDL_FreeCursor(cursor);
	}
}

void ModuleCamera3D::Focus()
{
	//if (App->objects->GetSelectedObject() != nullptr)
	//{
	//	AABB bounding_box = App->objects->GetSelectedObject()->GetBB();

	//	if (bounding_box.Diagonal().Length() != 0)
	//	{
	//		float offset = bounding_box.Diagonal().Length() * 0.7;
	//		float3 offset_v = fake_camera->Z * offset;
	//		fake_camera->Reference.x = bounding_box.CenterPoint().x;
	//		fake_camera->Reference.y = bounding_box.CenterPoint().y;
	//		fake_camera->Reference.z = bounding_box.CenterPoint().z;
	//		fake_camera->Position = fake_camera->Reference + offset_v;
	//	}
	//	else
	//	{
	//		float offset = 5.F;
	//		float3 offset_v = fake_camera->Z * offset;

	//		ComponentTransform* transform = (ComponentTransform*)App->objects->GetSelectedObject()->GetComponent(ComponentType::TRANSFORM);

	//		fake_camera->Reference.x = transform->GetGlobalPosition().x;
	//		fake_camera->Reference.y = transform->GetGlobalPosition().y;
	//		fake_camera->Reference.z = transform->GetGlobalPosition().z;

	//		fake_camera->Position = fake_camera->Reference + offset_v;
	//	}
	//}
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



