#include "Move.h"

Move::Move() : Alien()
{

}

Move::~Move()
{

}

void Move::Start()
{

}

void Move::Update()
{
	//if (Input::GetFirstKeyDown() == SDL_SCANCODE_1)
	//	test = 12;
	//if (Input::GetKey(SDL_SCANCODE_2) == Input::KEY_DOWN)
	//	test = 13;
	//if (Input::GetKeyRepeat(SDL_SCANCODE_3))
	//	test = 14;
	//if (Input::GetMouseButton(Input::MOUSE_LEFT_BUTTON) == Input::KEY_DOWN)
	//	test = 15;

	//Debug::Log("%i", Random::GetRandomIntBetweenTwo(1, 5));
	//Debug::Log("%f", Random::GetRandomFloatBetweenTwo(1, 5));

	//if (Time::GetGameTime() != Time::GetTimeSinceStart()) {
	//	Time::GetDT();
	//}

	//Debug::Log("Time: %f", Time::GetTimeSinceStart());
	//Camera::GetAllCameras();
	//Camera::GetCurrentCamera();
	//Camera::GetNumCameras();

	//ComponentLight* light = new ComponentLight(NULL);
	//light->ambient = Color::Yellow();

	//ComponentMesh* mesh = new ComponentMesh(nullptr);
	//mesh->IsEnabled();

	//ComponentScript* script = new ComponentScript(nullptr);
	//script->SetEnable(false);

	//GameObject* pl = GameObject::FindWithTag("Player");
	//game_object->SetName("NewNAME");
	Debug::Log(game_object->GetTag());
	//Debug::Log(pl->tag.data());
	//GameObject* en = GameObject::FindWithTag("Enemy");
	////GameObject* namee = GameObject::FindWithName("Light");
	//std::vector<GameObject*> untag = GameObject::FindGameObjectsWithTag("UnTagged");
	//std::vector<GameObject*> players = GameObject::FindGameObjectsWithTag("Player");
	//std::vector<GameObject*> enemies = GameObject::FindGameObjectsWithTag("Enemy");

}

void Move::OnDrawGizmos()
{
	Gizmos::DrawCube(transform->GetGlobalPosition(), { 4,2,3 }, Color::Yellow());
}
