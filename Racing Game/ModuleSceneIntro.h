#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void CreateObjects();
	void ChangeTime();
public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/
	float InitTime = 0;
	bool InMinutes = false;
	int Min = 0;
	PhysBody3D* sensor = nullptr;
	bool InSecondLap = false;
	bool FinalLap = false;
	int MinBeat = 3;
	int TimeAfterFinish = 3000;
	int TimeFinish = 0;
	int TimeMeta = 5000;
	int TimeColl = 0;
	int SecondBeat = 00;

	p2DynArray<Cube> bodies;


	PhysBody3D* body2;
	PhysBody3D* body;
	PhysBody3D* body3;
	PhysBody3D* body4;
	PhysBody3D* body5;
	PhysBody3D* body6;
	PhysBody3D* body7;
	PhysBody3D* body8;
	PhysBody3D* body9;
	PhysBody3D* body10;
	PhysBody3D* body11;
	PhysBody3D* body12;
	PhysBody3D* body13;
	PhysBody3D* body14;
	PhysBody3D* body15;
	PhysBody3D* body16;
	PhysBody3D* body17;
	PhysBody3D* body18;

	Cube cube18{ 3, 16, 3 };
	Cube cube19{ 3, 16, 3 };
	Cube cube20{ 3, 16, 3 };
	Cube cube10000{ 3, 16, 3 };
	Cube cube11000{ 3, 30, 3 };
	Cube cube11100{ 3, 16, 3 };
	Cube cube11110{ 3, 16, 3 };
	Cube cube21{3, 16, 3};
	Cube cube22{ 3,15,3 };

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	unsigned int win;
	unsigned int lose;
	bool audios=false;
};
