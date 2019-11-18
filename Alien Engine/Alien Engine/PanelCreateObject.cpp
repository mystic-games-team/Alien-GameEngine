#include "Application.h"
#include "PanelCreateObject.h"
#include "GameObject.h"
#include "ModuleObjects.h"
#include "Shapes.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ReturnZ.h"
#include "ComponentTransform.h"

PanelCreateObject::PanelCreateObject(const std::string& panel_name, const SDL_Scancode& key1_down, const SDL_Scancode& key2_repeat, const SDL_Scancode& key3_repeat_extra) :
	Panel(panel_name, key1_down, key2_repeat, key3_repeat_extra)
{
	shortcut = App->shortcut_manager->AddShortCut("CreateObject", key1_down, std::bind(&Panel::ChangeEnable, this), key2_repeat, key3_repeat_extra);
}

PanelCreateObject::~PanelCreateObject()
{
}

void PanelCreateObject::PanelLogic()
{
	static float x = 0.0f;
	static float y = 0.f;
	static float z = 0.f;

	static int objects_combo = 0;

	ImGui::OpenPopup(panel_name.c_str());
	ImGui::SetNextWindowContentWidth(300);
	if (ImGui::BeginPopupModal(panel_name.c_str(), &enabled, ImGuiWindowFlags_NoResize| ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::IsWindowHovered())
			App->camera->is_scene_hovered = false;
		ImGui::Spacing();
		ImGui::Spacing();

		
		ImGui::Combo("Select Object", &objects_combo, "Cube\0Sphere\0Rock\0Dodecahedron\0Icosahedron\0Octahedron\0Torus\0Klein Bottle\0");

		ImGui::Spacing();
		
		ImGui::ColorEdit3("Select Color", (float*)&create_color, ImGuiColorEditFlags_Float);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Position");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Columns(3,0,false);
		ImGui::InputFloat("X", &x, 0, 0, 2);
		ImGui::NextColumn();
		ImGui::InputFloat("Y", &y, 0, 0, 2);
		ImGui::NextColumn();
		ImGui::InputFloat("Z", &z, 0, 0, 2);
		ImGui::NextColumn();
		ImGui::Columns(1);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();


		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Create", { ImGui::GetWindowWidth()-16,25 }))
		{
			GameObject* object = new GameObject(App->objects->base_game_object);
			ComponentTransform* transform = new ComponentTransform(object, { x,y,z }, { 0,0,0,0 }, { 1,1,1 });
			ComponentMesh* mesh = new ComponentMesh(object);
			ComponentMaterial* material = new ComponentMaterial(object);
			switch (objects_combo)
			{
			case 0: {
				object->SetName("Cube");
				mesh->mesh = App->resources->cube;
				break; }
			case 1: {
				mesh->mesh = App->resources->sphere;
				object->SetName("Sphere");
				break; }
			case 2: {
				mesh->mesh = App->resources->rock;
				object->SetName("Rock");
				break; }
			case 3: {
				mesh->mesh = App->resources->dodecahedron;
				object->SetName("Dodecahedron");
				break; }
			case 4: {
				mesh->mesh = App->resources->icosahedron;
				object->SetName("Icosahedron");
				break; }
			case 5: {
				mesh->mesh = App->resources->octahedron;
				object->SetName("Octahedron");
				break; }
			case 6: {
				mesh->mesh = App->resources->torus;
				object->SetName("Torus");
				break; }
			}
			x = y = z = 0;
			material->color = create_color;
			object->AddComponent(transform);
			object->AddComponent(mesh);
			object->AddComponent(material);
			mesh->RecalculateAABB_OBB();
			App->objects->SetNewSelectedObject(object);
			ReturnZ::AddNewAction(ReturnZ::ReturnActions::ADD_OBJECT, object);
			create_color = { 1,1,1 };
			ChangeEnable();
		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::EndPopup();
	}
}