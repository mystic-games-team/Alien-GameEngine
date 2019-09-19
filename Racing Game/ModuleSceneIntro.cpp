#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "PhysBody3D.h"
#include "Primitive.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "p2String.h"
#include "ModuleAudio.h"
#include "Primitive.h"
#include "ModuleAudio.h"
ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	InitTime = SDL_GetTicks();
	CreateObjects();
	InSecondLap = false;
	FinalLap = false;
	win=App->audio->LoadFx("audio/WIN.wav");
	lose= App->audio->LoadFx("audio/LOSE.wav");
	App->audio->PlayMusic("audio/SONG.ogg");
	App->player->CanInput = true;
	audios = true;
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	
	//body2->GetTransform(&cube19.transform);
	body->GetTransform(&cube18.transform);
	cube18.Render();
	body4->GetTransform(&cube19.transform);
	cube19.Render();
	body6->GetTransform(&cube20.transform);
	cube20.Render();
	body8->GetTransform(&cube10000.transform);
	cube10000.Render();
	body9->GetTransform(&cube11000.transform);
	cube11000.Render();
	body11->GetTransform(&cube11100.transform);
	cube11100.Render();
	body15->GetTransform(&cube21.transform);
	cube21.Render();
	body18->GetTransform(&cube22.transform);
	cube22.Render();
	for (int i = 0; i < bodies.Count(); ++i) {
		bodies[i].Render();
	}
	
	
	ChangeTime();
	if (FinalLap) {
		if (Min <= MinBeat && (SDL_GetTicks() - InitTime) / 1000 < SecondBeat) {
			MinBeat = Min;
			SecondBeat = (SDL_GetTicks() - InitTime) / 1000;
			if (audios) {
				App->audio->PlayFx(win);
				audios = false;
			}
		}
		else {
			Min = 0;
			if (audios) {
				App->audio->PlayFx(lose);
				audios = false;
			}
		}
		App->player->CanInput = false;
		if (SDL_GetTicks() - TimeFinish > TimeAfterFinish) {
			FinalLap = false;
			App->physics->CleanUp();
			App->player->CleanUp();
			CleanUp();
			App->physics->Start();
			App->player->Start();
			Start();
		}
	}
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1 == sensor && !InSecondLap) {
		InSecondLap = true;
		TimeColl = SDL_GetTicks();
	}
	if (body1 == sensor && InSecondLap && !FinalLap && SDL_GetTicks() - TimeColl > TimeMeta) {
		FinalLap = true;
		TimeFinish = SDL_GetTicks();
	}
}

void ModuleSceneIntro::CreateObjects()
{

	Cube cube1000(1000, 0, 1000); //terra
	cube1000.SetPos(0, 0, 0);
	cube1000.color = Black;
	App->physics->AddBody(cube1000, 0.0f);
	bodies.PushBack(cube1000);

	Cube cube(100, 7, 50); //el que tens davant al entrar
	cube.SetPos(0, 0, -25);
	cube.color = White;
	App->physics->AddBody(cube, 0.0f);
	bodies.PushBack(cube);
	
	Cube cube6(38, 7, 5); //darrera teu al entrar
	cube6.color = White;
	cube6.SetPos(3.5f, 0, 20);
	App->physics->AddBody(cube6, 0.0f);
	bodies.PushBack(cube6);

	Cube cube2(5, 7, 30); //esquerra al entrar
	cube2.color = White;
	cube2.SetPos(-30, 0, 10);
	App->physics->AddBody(cube2, 0.0f);
	bodies.PushBack(cube2);

	Cube cube3(5, 7, 30); //dreta al entrar
	cube3.color = White;
	cube3.SetPos(25, 0, 15);
	App->physics->AddBody(cube3, 0.0f);
	bodies.PushBack(cube3);

	Cube cube4(5, 7, 30); //esquerra2 al entrar
	cube4.color = White;
	cube4.SetPos(-25, 0, 40);
	App->physics->AddBody(cube4, 0.0f);
	bodies.PushBack(cube4);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Cube cube7(5, 7, 20); //dreta3 al entrar
	cube7.color = White;
	cube7.SetPos(20, 0, 65);
	App->physics->AddBody(cube7, 0.0f);
	bodies.PushBack(cube7);

	Cube cube8(5, 7, 30); //esquerra3 al entrar
	cube8.color = White;
	cube8.SetPos(-20, 0, 60);
	App->physics->AddBody(cube8, 0.0f);
	bodies.PushBack(cube8);

	Cube cube9(5, 7, 30); //dreta4 al entrar
	cube9.color = White;
	cube9.SetPos(15, 0, 80);
	App->physics->AddBody(cube9, 0.0f);
	bodies.PushBack(cube9);

	Cube cube10(5, 7, 30); //esquerra4 al entrar
	cube10.color = White;
	cube10.SetPos(-15, 0, 80);
	App->physics->AddBody(cube10, 0.0f);
	bodies.PushBack(cube10);

	Cube cube11(5, 7, 30); //dreta5 al entrar
	cube11.color = White;
	cube11.SetPos(10, 0, 100);
	App->physics->AddBody(cube11, 0.0f);
	bodies.PushBack(cube11);

	Cube cube12(5, 7, 30); //esquerra5 al entrar
	cube12.color = White;
	cube12.SetPos(-10, 0, 100);
	App->physics->AddBody(cube12, 0.0f);
	bodies.PushBack(cube12);

	Cube cube13(5, 7, 30); //dreta6 al entrar
	cube13.color = White;
	cube13.SetPos(5, 0, 120);
	App->physics->AddBody(cube13, 0.0f);
	bodies.PushBack(cube13);

	Cube cube14(5, 7, 30); //esquerra6 al entrar
	cube14.color = White;
	cube14.SetPos(-5, 0, 120);
	App->physics->AddBody(cube14, 0.0f);
	bodies.PushBack(cube14);

	//COMENÇAMENT DE LA CURVA A LA DRETA 
	Cube cube15(5, 7, 50); 
	cube15.color = White;
	cube15.SetPos(5, 0, 140);
	App->physics->AddBody(cube15, 0.0f);
	bodies.PushBack(cube15);

	Cube cube16(5, 7, 100);
	cube16.color = White;
	cube16.SetPos(-82.5f, 0, 167.5f);
	cube16.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube16, 0.0f);
	bodies.PushBack(cube16);

	Cube cube17(5, 7, 80);
	cube17.color = White;
	cube17.SetPos(-82.5f, 0, 137.5f);
	cube17.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube17, 0.0f);
	bodies.PushBack(cube17);
	//FINAL RECTE 

	Cube cube30(5, 7, 100);
	cube30.color = White;
	cube30.SetPos(-42.5f, 0, 167.5f);
	cube30.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube30, 0.0f);
	bodies.PushBack(cube30);

	Cube cube31(5, 7, 80);
	cube31.color = White;
	cube31.SetPos(-42.5f, 0, 137.5f);
	cube31.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube31, 0.0f);
	bodies.PushBack(cube31);

	Cube cube32(5, 7, 80);
	cube32.color = White;
	cube32.SetPos(-120.5f, 0, 167.5f);
	cube32.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube32, 0.0f);
	bodies.PushBack(cube32);

	Cube cube33(5, 7, 160);
	cube33.color = White;
	cube33.SetPos(-163, 0, 90);
	App->physics->AddBody(cube33, 0.0f);
	bodies.PushBack(cube33);

	Cube cube34(5, 7, 140);
	cube34.color = White;
	cube34.SetPos(-120, 0, 67.5f);
	App->physics->AddBody(cube34, 0.0f);
	bodies.PushBack(cube34);

	Cube cube35(5, 7, 100);
	cube35.color = White;
	cube35.SetPos(-167.5f, 0, -5);
	cube35.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube35, 0.0f);
	bodies.PushBack(cube35);

	Cube cube36(5, 7, 80);
	cube36.color = White;
	cube36.SetPos(-200.5f, 0, 7.5f);
	cube36.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube36, 0.0f);
	bodies.PushBack(cube36);

	

	Cube cube38(5, 7, 80);
	cube38.color = White;
	cube38.SetPos(-242.8f, 0, -30.5f);
	App->physics->AddBody(cube38, 0.0f);
	bodies.PushBack(cube38);

	Cube cube39(5, 7, 80);
	cube39.color = White;
	cube39.SetPos(-219.8f, 0, -42.5f);
	App->physics->AddBody(cube39, 0.0f);
	bodies.PushBack(cube39);

	Cube cube40(5, 7, 80);
	cube40.color = White;
	cube40.SetPos(-242.8f, 0, -70.5f);
	App->physics->AddBody(cube40, 0.0f);
	bodies.PushBack(cube40);

	Cube cube41(5, 7, 200);
	cube41.color = White;
	cube41.SetPos(-145, 0, -112.5f);
	cube41.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube41, 0.0f);
	bodies.PushBack(cube41);

	Cube cube42(5, 7, 200);
	cube42.color = White;
	cube42.SetPos(-117.5f, 0, -80);
	cube42.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube42, 0.0f);
	bodies.PushBack(cube42);

	Cube cube43(5, 7, 100);
	cube43.color = White;
	cube43.SetPos(-15.5f, 0, -127.5f);
	App->physics->AddBody(cube43, 0.0f);
	bodies.PushBack(cube43);

	Cube cube44(5, 7, 180);
	cube44.color = White;
	cube44.SetPos(-42.5f, 0, -200);
	App->physics->AddBody(cube44, 0.0f);
	bodies.PushBack(cube44);

	Cube cube45(5, 7, 80);
	cube45.color = White;
	cube45.SetPos(-15.5f, 0, -217.5f);
	App->physics->AddBody(cube45, 0.0f);
	bodies.PushBack(cube45);

	Cube cube46(5, 7, 80);
	cube46.color = White;
	cube46.SetPos(22, 0, -257.5f);
	cube46.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube46, 0.0f);
	bodies.PushBack(cube46);

	Cube cube47(5, 7, 80);
	cube47.color = White;
	cube47.SetPos(22, 0, -257.5f);
	cube47.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube47, 0.0f);
	bodies.PushBack(cube47);

	Cube cube48(5, 7, 120);
	cube48.color = White;
	cube48.SetPos(19.5f, 0, -287.5f);
	cube48.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube48, 0.0f);
	bodies.PushBack(cube48);

	Cube cube49(5, 7, 340);
	cube49.color = White;
	cube49.SetPos(82, 0, -120);
	App->physics->AddBody(cube49, 0.0f);
	bodies.PushBack(cube49);

	Cube cube50(5, 7, 290);
	cube50.color = White;
	cube50.SetPos(60, 0, -115);
	App->physics->AddBody(cube50, 0.0f);
	bodies.PushBack(cube50);

	Cube cube51(5, 7, 40);
	cube51.color = White;
	cube51.SetPos(42.5f, 0, 32.5f);
	cube51.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube51, 0.0f);
	bodies.PushBack(cube51);

	Cube cube52(5, 7, 65);
	cube52.color = White;
	cube52.SetPos(53, 0, 52.5f);
	cube52.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube52, 0.0f);
	bodies.PushBack(cube52);

	//SENSOR
	
	Cube cube3432(20, 7, 0);
	cube3432.color = Green;
	cube3432.SetPos(24.5f, 0, 40);
	cube3432.SetRotation(90, { 0,1,0 });
	sensor = App->physics->AddBody(cube3432, 0.0f, true);
	sensor->collision_listeners.add(this);

	//OBSTACLES
	Cube cube37(8, 7, 2);
	cube37.color = Green;
	cube37.SetPos(-78.5f, 0, 153);
	cube37.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube37, 0.0f);
	bodies.PushBack(cube37);

	Cube cube999(12, 7, 2);
	cube999.color = Green;
	cube999.SetPos(-152.5f, 0, 122);
	App->physics->AddBody(cube999, 0.0f);
	bodies.PushBack(cube999);

	Cube cube998(12, 7, 2);
	cube998.color = Green;
	cube998.SetPos(-129.5f, 0, 109);
	App->physics->AddBody(cube998, 0.0f);
	bodies.PushBack(cube998);

	Cube cube997(12, 7, 2);
	cube997.color = Green;
	cube997.SetPos(-227.5f, 0, -10);
	App->physics->AddBody(cube997, 0.0f);
	bodies.PushBack(cube997);

	Cube cube996(12, 7, 2);
	cube996.color = Green;
	cube996.SetPos(-235.5f, 0, -40);
	App->physics->AddBody(cube996, 0.0f);
	bodies.PushBack(cube996);

	Cube cube995(12, 7, 2);
	cube995.color = Green;
	cube995.SetPos(-227.5f, 0, -70);
	App->physics->AddBody(cube995, 0.0f);
	bodies.PushBack(cube995);

	Cube cube994(12, 7, 2);
	cube994.color = Green;
	cube994.SetPos(-210.5f, 0, -105);
	cube994.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube994, 0.0f);
	bodies.PushBack(cube994);

	Cube cube993(12, 7, 2);
	cube993.color = Green;
	cube993.SetPos(-205.5f, 0, -88);
	cube993.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube993, 0.0f);
	bodies.PushBack(cube993);

	Cube cube992(6, 10, 4);
	cube992.color = Green;
	cube992.SetPos(-197.5f, 0, -96.5f);
	cube992.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube992, 0.0f);
	bodies.PushBack(cube992);

	Cube cube991(20, 7, 2);
	cube991.color = Green;
	cube991.SetPos(-150.5f, 0, -104);
	cube991.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube991, 0.0f);
	bodies.PushBack(cube991);

	Cube cube990(6, 7, 2);
	cube990.color = Green;
	cube990.SetPos(-150.5f, 0, -85);
	cube990.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube990, 0.0f);
	bodies.PushBack(cube990);

	Cube cube987(7, 7, 2);
	cube987.color = Green;
	cube987.SetPos(-115, 0, -96);
	cube987.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube987, 0.0f);
	bodies.PushBack(cube987);

	Cube cube989(10, 7, 2);
	cube989.color = Green;
	cube989.SetPos(-80, 0, -105);
	cube989.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube989, 0.0f);
	bodies.PushBack(cube989);

	Cube cube988(10, 7, 2);
	cube988.color = Green;
	cube988.SetPos(-80, 0, -87);
	cube988.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube988, 0.0f);
	bodies.PushBack(cube988);

	Cube cube986(15, 7, 2);
	cube986.color = Green;
	cube986.SetPos(-28, 0, -96);
	cube986.SetRotation(135, {0 ,1,0 });
	App->physics->AddBody(cube986, 0.0f);
	bodies.PushBack(cube986);

	Cube cube985(15, 7, 2);
	cube985.color = Green;
	cube985.SetPos(-33.5f, 0, -150);
	App->physics->AddBody(cube985, 0.0f);
	bodies.PushBack(cube985);

	Cube cube984(12, 7, 2);
	cube984.color = Green;
	cube984.SetPos(-23.5f, 0, -180);
	App->physics->AddBody(cube984, 0.0f);
	bodies.PushBack(cube984);

	Cube cube983(10, 7, 2);
	cube983.color = Green;
	cube983.SetPos(-28.5f, 0,-220);
	App->physics->AddBody(cube983, 0.0f);
	bodies.PushBack(cube983);

	Cube cube982(10, 7, 2);
	cube982.color = Green;
	cube982.SetPos(-37.5f, 0, -255);
	App->physics->AddBody(cube982, 0.0f);
	bodies.PushBack(cube982);

	Cube cube981(10, 7, 2);
	cube981.color = Green;
	cube981.SetPos(-20.5f, 0, -255);
	App->physics->AddBody(cube981, 0.0f);
	bodies.PushBack(cube981);

	Cube cube980(10, 7, 2);
	cube980.color = Green;
	cube980.SetPos(40, 0, -263);
	cube980.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube980, 0.0f);
	bodies.PushBack(cube980);

	Cube cube979(10, 7, 2);
	cube979.color = Green;
	cube979.SetPos(40, 0, -282);
	cube979.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube979, 0.0f);
	bodies.PushBack(cube979);

	Cube cube978(12, 7, 2);
	cube978.color = Green;
	cube978.SetPos(60, 0, -263);
	cube978.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube978, 0.0f);
	bodies.PushBack(cube978);

	Cube cube977(12, 7, 2);
	cube977.color = Green;
	cube977.SetPos(60, 0, -280);
	cube977.SetRotation(90, { 0,1,0 });
	App->physics->AddBody(cube977, 0.0f);
	bodies.PushBack(cube977);

	Cube cube976(7, 7, 2);
	cube976.color = Green;
	cube976.SetPos(66, 0, -255);
	App->physics->AddBody(cube976, 0.0f);
	bodies.PushBack(cube976);

	Cube cube975(7, 7, 2);
	cube975.color = Green;
	cube975.SetPos(76, 0, -245);
	App->physics->AddBody(cube975, 0.0f);
	bodies.PushBack(cube975);

	Cube cube974(7, 7, 2);
	cube974.color = Green;
	cube974.SetPos(66, 0, -235);
	App->physics->AddBody(cube974, 0.0f);
	bodies.PushBack(cube974);

	Cube cube973(12, 7, 2);
	cube973.color = Green;
	cube973.SetPos(74, 0, -215);
	App->physics->AddBody(cube973, 0.0f);
	bodies.PushBack(cube973);

	Cube cube972(12, 7, 2);
	cube972.color = Green;
	cube972.SetPos(68, 0, -195);
	App->physics->AddBody(cube972, 0.0f);
	bodies.PushBack(cube972);

	Cube cube971(12, 7, 2);
	cube973.color = Green;
	cube973.SetPos(74, 0, -175);
	App->physics->AddBody(cube973, 0.0f);
	bodies.PushBack(cube973);

	Cube cube970(12, 7, 2);
	cube972.color = Green;
	cube972.SetPos(68, 0, -155);
	App->physics->AddBody(cube972, 0.0f);
	bodies.PushBack(cube972);

	Cube cube969(6, 7, 2);
	cube969.color = Green;
	cube969.SetPos(65, 0, -120);
	App->physics->AddBody(cube969, 0.0f);
	bodies.PushBack(cube969);

	Cube cube968(6, 7, 2);
	cube968.color = Green;
	cube968.SetPos(76, 0, -120);
	App->physics->AddBody(cube968, 0.0f);
	bodies.PushBack(cube968);

	Cube cube967(15, 7, 2);
	cube967.color = Green;
	cube967.SetPos(66, 0, -80);
	App->physics->AddBody(cube967, 0.0f);
	bodies.PushBack(cube967);

	Cube cube966(15, 7, 2);
	cube966.color = Green;
	cube966.SetPos(75, 0, -60);
	App->physics->AddBody(cube966, 0.0f);
	bodies.PushBack(cube966);


	//CONSTRAINTS
	//PRIMERA
	cube18.color = Red;
	cube18.SetPos(-42.5f, 3, 152.5f);
	cube18.SetRotation(90, vec3(0, 0, 1));
	body = App->physics->AddBody(cube18, 10000.0f);
	body->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	
	Cylinder cyl43(0.1f, 3);
	cyl43.SetPos(-33.5f, 20, 152.5f);
	cyl43.SetRotation(90, vec3(0, 0, 1));
	body2 = App->physics->AddBody(cyl43, 10000.0f);
	body2->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	
	App->physics->AddConstraintHinge(*body2, *body, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0),true, true);

	//SEGONA
	cube19.color = Red;
	cube19.SetPos(-92.5f, 10, 160.5f);
	cube19.SetRotation(90, vec3(0, 1, 0));
	body4 = App->physics->AddBody(cube19, 10000.0f);
	body4->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	Cylinder cyl2(0.1f, 3);
	cyl2.SetPos(-83.5f, 10, 160.5f);
	cyl2.SetRotation(90, vec3(0, 1, 0));
	body3 = App->physics->AddBody(cyl2, 10000.0f);
	body3->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	App->physics->AddConstraintHinge(*body3, *body4, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);

	//TERCERA
	cube20.color = Red;
	cube20.SetPos(-92.5f, 10, 144.5f);
	cube20.SetRotation(90, vec3(0, 1, 0));
	body6 = App->physics->AddBody(cube20, 10000.0f);
	body6->GetBody()->setLinearFactor(btVector3(0, 0, 0));
	
	Cylinder cyl3(0.1f, 3);
	cyl3.SetPos(-83.5f, 10, 144.5f);
	cyl3.SetRotation(90, vec3(0, 1, 0));
	body5 = App->physics->AddBody(cyl3, 10000.0f);
	body5->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	App->physics->AddConstraintHinge(*body5, *body6, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);


	cube10000.color = Red;
	cube10000.SetPos(-135.5f, 3, 122.5f);
	cube10000.SetRotation(90, vec3(0, 0, 1));
	body8 = App->physics->AddBody(cube10000, 10000.0f);
	body8->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	Cylinder cyl1(0.1f, 3);
	cyl1.SetPos(-133.5f, 20, 122.5f);
	cyl1.SetRotation(90, vec3(0, 0, 1));
	body7 = App->physics->AddBody(cyl1, 10000.0f);
	body7->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	App->physics->AddConstraintHinge(*body7, *body8, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);

	cube11000.color = Red;
	cube11000.SetPos(-140.5f, 3, 60.5f);
	cube11000.SetRotation(90, vec3(0, 0, 1));
	body9 = App->physics->AddBody(cube11000, 10000.0f);
	body9->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	Cylinder cyl54321(0.1f, 3);
	cyl54321.SetPos(-140.5f, 20, 60.5f);
	cyl54321.SetRotation(90, vec3(0, 0, 1));
	body10 = App->physics->AddBody(cyl54321, 10000.0f);
	body10->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	App->physics->AddConstraintHinge(*body10, *body9, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);

	cube11100.color = Red;
	cube11100.SetPos(-175.5f, 3, -94.5f);
	cube11100.SetRotation(90, vec3(0, 0, 1));
	body11 = App->physics->AddBody(cube11100, 10000.0f);
	body11->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	Cylinder cyl543221(0.1f, 3);
	cyl543221.SetPos(-175.5f, 20, -94.5f);
	cyl543221.SetRotation(90, vec3(0, 0, 1));
	body12 = App->physics->AddBody(cyl543221, 10000.0f);
	body12->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	App->physics->AddConstraintHinge(*body12, *body11, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);

	
	cube21.color = Red;
	cube21.SetPos(10, 3, -270);
	cube21.SetRotation(90, vec3(0, 0, 1));
	body15 = App->physics->AddBody(cube21, 10000.0f);
	body15->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	Cylinder cyl45(0.1f, 3);
	cyl45.SetPos(19, 20, -270);
	cyl45.SetRotation(90, vec3(0, 0, 1));
	body16 = App->physics->AddBody(cyl45, 10000.0f);
	body16->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	App->physics->AddConstraintHinge(*body16, *body15, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);

	cube22.color = Red;
	cube22.SetPos(70, 3, 0);
	cube22.SetRotation(90, vec3(0, 0, 1));
	body18 = App->physics->AddBody(cube22, 10000.0f);
	body18->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	Cylinder cyl46(0.1f, 3);
	cyl46.SetPos(70, 20, 00);
	cyl46.SetRotation(90, vec3(0, 0, 1));
	body17 = App->physics->AddBody(cyl46, 10000.0f);
	body17->GetBody()->setLinearFactor(btVector3(0, 0, 0));

	App->physics->AddConstraintHinge(*body17, *body18, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);
}

void ModuleSceneIntro::ChangeTime()
{

	if ((SDL_GetTicks() - InitTime) / 1000 >= 60) {
		Min++;
		InitTime = SDL_GetTicks();
	}

	p2SString StringTime;
	if (Min > 9 && ((SDL_GetTicks() - InitTime) / 1000) > 9) {
		if (SecondBeat < 10) {
			StringTime.create("SUPER RACING GAME || Time To Beat: 0%i : 0%i - YourTime: %i : %.2f", MinBeat, SecondBeat, Min, (SDL_GetTicks() - InitTime) / 1000);
		}
		else
			StringTime.create("SUPER RACING GAME || Time To Beat: 0%i : %i - YourTime: %i : %.2f", MinBeat, SecondBeat, Min, (SDL_GetTicks() - InitTime) / 1000);
	}
	else if (Min > 9 && ((SDL_GetTicks() - InitTime) / 1000) <= 9) {
		if (SecondBeat < 10) {
			StringTime.create("SUPER RACING GAME || Time To Beat: 0%i : 0%i - YourTime: %i : 0%.2f", MinBeat, SecondBeat, Min, (SDL_GetTicks() - InitTime) / 1000);
		}
		else
			StringTime.create("SUPER RACING GAME || Time To Beat: 0%i : %i - YourTime: %i : 0%.2f", MinBeat, SecondBeat, Min, (SDL_GetTicks() - InitTime) / 1000);
	}
	else if (Min <= 9 && ((SDL_GetTicks() - InitTime) / 1000) <= 9) {
		if (SecondBeat < 10) {
			StringTime.create("SUPER RACING GAME || Time To Beat: 0%i : 0%i - YourTime: 0%i : 0%.2f", MinBeat, SecondBeat, Min, (SDL_GetTicks() - InitTime) / 1000);
		}
		else 
			StringTime.create("SUPER RACING GAME || Time To Beat: 0%i : %i - YourTime: 0%i : 0%.2f", MinBeat, SecondBeat, Min, (SDL_GetTicks() - InitTime) / 1000);
	}
	else {
		if (SecondBeat < 10) {
			StringTime.create("SUPER RACING GAME || Time To Beat: 0%i : 0%i - YourTime: 0%i : %.2f", MinBeat, SecondBeat, Min, (SDL_GetTicks() - InitTime) / 1000);
		}
		else
			StringTime.create("SUPER RACING GAME || Time To Beat: 0%i : %i - YourTime: 0%i : %.2f", MinBeat, SecondBeat, Min, (SDL_GetTicks() - InitTime) / 1000);
	}
		
	App->window->SetTitle(StringTime.GetString());
}


