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
{}

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
	newPos = { 0,0,0 };

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
		Rotation();
	}
	if (is_scene_hovered || is_scene_focused) 
	{
		Movement();
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		Focus();

	fake_camera->Position += newPos;
	fake_camera->Reference += newPos;

	// Recalculate matrix -------------
	fake_camera->CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3& Position, const float3& Reference, bool RotateAroundReference)
{
	fake_camera->Position = Position;
	fake_camera->Reference = Reference;

	
	fake_camera->Z = Position.Normalized() - Reference.Normalized();
	fake_camera->X = (float3(0.0f, 1.0f, 0.0f).Cross(fake_camera->Z)).Normalized();
	fake_camera->Y = fake_camera->Z.Cross(fake_camera->X);

	if(!RotateAroundReference)
	{
		fake_camera->Reference = fake_camera->Position;
		fake_camera->Position += fake_camera->Z * 0.05f;
	}

	fake_camera->CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3 &Spot)
{
	fake_camera->Reference = Spot;

	fake_camera->Z = fake_camera->Position.Normalized() - fake_camera->Reference.Normalized();
	fake_camera->X = (float3(0.0f, 1.0f, 0.0f).Cross(fake_camera->Z)).Normalized();
	fake_camera->Y = fake_camera->Z.Cross(fake_camera->X);

	fake_camera->CalculateViewMatrix();
}


// -----------------------------------------------------------------


void ModuleCamera3D::Movement()
{
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) newPos.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= fake_camera->Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += fake_camera->Z * speed;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= fake_camera->X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += fake_camera->X * speed;
	}

	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
		SDL_SetCursor(cursor);

		if (App->input->GetMouseXMotion() > -1) newPos -= fake_camera->X * mouse_speed;
		if (App->input->GetMouseXMotion() < 1) newPos += fake_camera->X * mouse_speed;

		if (App->input->GetMouseYMotion() < 1) newPos -= fake_camera->Y * mouse_speed*0.5f ;
		if (App->input->GetMouseYMotion() > -1) newPos += fake_camera->Y * mouse_speed*0.5f;

		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_UP)
		{
			SDL_FreeCursor(cursor);
		}
	}
}

void ModuleCamera3D::Zoom()
{
	if (App->input->GetMouseZ() > 0)
	{
		newPos -= fake_camera->Z * zoom_speed;
	}
	else if (App->input->GetMouseZ() < 0)
	{
		newPos += fake_camera->Z * zoom_speed;
	}
	fake_camera->Reference -= newPos;
}

void ModuleCamera3D::Rotation()
{
	//if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT)==KEY_REPEAT)
	//{
	//	int dx = -App->input->GetMouseXMotion();
	//	int dy = -App->input->GetMouseYMotion();

	//	float Sensitivity = 0.25f;

	//	fake_camera->Position -= fake_camera->Reference;

	//	cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
	//	SDL_SetCursor(cursor);

	//	if (dx != 0)
	//	{
	//		float DeltaX = (float)dx * Sensitivity;

	//		
	//		fake_camera->X = rotate(fake_camera->X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//		fake_camera->Y = rotate(fake_camera->Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//		fake_camera->Z = rotate(fake_camera->Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	//	}

	//	if (dy != 0)
	//	{
	//		float DeltaY = (float)dy * Sensitivity;

	//		fake_camera->Y = rotate(Y, DeltaY, X);
	//		fake_camera->Z = rotate(Z, DeltaY, X);

	//		if (Y.y < 0.0f)
	//		{
	//			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//			Y = cross(Z, X);
	//		}
	//	}

	//	fake_camera->Position = fake_camera->Reference + fake_camera->Z * fake_camera->Position.Length();

	//	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	//	{
	//		SDL_FreeCursor(cursor);
	//	}
	//}
}

void ModuleCamera3D::Focus()
{
	if (App->objects->GetSelectedObject() != nullptr)
	{
		AABB bounding_box = App->objects->GetSelectedObject()->GetBB();

		if (bounding_box.Diagonal().Length() != 0)
		{
			float offset = bounding_box.Diagonal().Length() * 0.7;
			float3 offset_v = fake_camera->Z * offset;
			fake_camera->Reference.x = bounding_box.CenterPoint().x;
			fake_camera->Reference.y = bounding_box.CenterPoint().y;
			fake_camera->Reference.z = bounding_box.CenterPoint().z;
			fake_camera->Position = fake_camera->Reference + offset_v;
		}
		else
		{
			float offset = 5.F;
			float3 offset_v = fake_camera->Z * offset;

			ComponentTransform* transform = (ComponentTransform*)App->objects->GetSelectedObject()->GetComponent(ComponentType::TRANSFORM);

			fake_camera->Reference.x = transform->GetGlobalPosition().x;
			fake_camera->Reference.y = transform->GetGlobalPosition().y;
			fake_camera->Reference.z = transform->GetGlobalPosition().z;

			fake_camera->Position = fake_camera->Reference + offset_v;
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



