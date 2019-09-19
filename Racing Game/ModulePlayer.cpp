#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "ModuleWindow.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(3, 2, 6);
	car.chassis_offset.Set(0, 1, 0);
	car.mass = 300.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;
	car.aileron_offset.Set(0, 2.5f, 3);
	car.aileron_radius = 0.5f;
	car.aileron_height = 3.5f;
	
	car.num_lights = 4;
	car.lights = new Lights[car.num_lights];
	//LIGHT DRETA
	car.lights[0].dimensions.Set(0.7, 0.7, 1.0);
	car.lights[0].offset.Set(0.8f, 2.3, -2.6f);
	//LIGHT ESQUERRA
	car.lights[1].dimensions.Set(0.7, 0.7, 1.0);
	car.lights[1].offset.Set(-0.8f, 2.3, -2.6f);
	//CENTRAL
	car.lights[2].dimensions.Set(3, 1, 1.0);
	car.lights[2].offset.Set(0.0f, 1.0, -2.6f);
	//CENTRAL 2
	car.lights[3].dimensions.Set(3, 0.6f, 2.0);
	car.lights[3].offset.Set(0.0f, 0.3, -2.6f);

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;

	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 6;
	car.wheels = new Wheel[car.num_wheels];

	// DARRERA DRETA
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = false;
	car.wheels[0].drive = true;
	car.wheels[0].brake = true;
	car.wheels[0].steering = false;

	// DARRERA DRETA2
	car.wheels[4].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius - 2.5f);
	car.wheels[4].direction = direction;
	car.wheels[4].axis = axis;
	car.wheels[4].suspensionRestLength = suspensionRestLength;
	car.wheels[4].radius = wheel_radius;
	car.wheels[4].width = wheel_width;
	car.wheels[4].front = false;
	car.wheels[4].drive = true;
	car.wheels[4].brake = true;
	car.wheels[4].steering = false;

	// DARRERA ESQUERRA
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = false;
	car.wheels[1].drive = true;
	car.wheels[1].brake = true;
	car.wheels[1].steering = false;

	// DARRERA ESQUERRA 2
	car.wheels[5].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius - 2.5f);
	car.wheels[5].direction = direction;
	car.wheels[5].axis = axis;
	car.wheels[5].suspensionRestLength = suspensionRestLength;
	car.wheels[5].radius = wheel_radius;
	car.wheels[5].width = wheel_width;
	car.wheels[5].front = false;
	car.wheels[5].drive = true;
	car.wheels[5].brake = true;
	car.wheels[5].steering = false;

	// DRETA DAVANT 
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = true;
	car.wheels[2].drive = true;
	car.wheels[2].brake = false;
	car.wheels[2].steering = true;

	// ESQUERRA DAVANT
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = true;
	car.wheels[3].drive = true;
	car.wheels[3].brake = false;
	car.wheels[3].steering = true;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 0, 10);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;
	if (CanInput) {
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		{
			brake = BRAKE_POWER * dt;
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				acceleration = MAX_ACCELERATION * dt;
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				acceleration = -MAX_ACCELERATION * dt;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{

			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}
	}
	

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	
	return UPDATE_CONTINUE;
}



