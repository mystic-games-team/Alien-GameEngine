#include "ComponentTransform.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "ModuleObjects.h"
#include "Application.h"
#include "ReturnZ.h"
#include "ComponentMesh.h"
#include "PanelScene.h"
#include "ResourcePrefab.h"
#include "PanelProject.h"

ComponentTransform::ComponentTransform(GameObject* attach) : Component(attach)
{
	type = ComponentType::TRANSFORM;
}

ComponentTransform::ComponentTransform(GameObject* attach, const float3& pos, const Quat& rot, const float3& scale) : Component(attach)
{
	local_position = pos;
	local_rotation = rot;
	local_scale = scale;
	euler_rotation = local_rotation.ToEulerXYZ();
	euler_rotation.x = RadToDeg(euler_rotation.x);
	euler_rotation.y = RadToDeg(euler_rotation.y);
	euler_rotation.z = RadToDeg(euler_rotation.z);
	local_transformation = float4x4::FromTRS(local_position, local_rotation, local_scale);

	if (game_object_attached->parent != nullptr) {
		ComponentTransform* tr = (ComponentTransform*)game_object_attached->parent->GetComponent(ComponentType::TRANSFORM);
		if (tr != nullptr) global_transformation = tr->global_transformation * local_transformation;
		else global_transformation = local_transformation;
	}
	else
		global_transformation = local_transformation;

	type = ComponentType::TRANSFORM;
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::SetLocalPosition(const float& x, const float& y, const float& z)
{
	local_position.x = x;
	local_position.y = y;
	local_position.z = z;

	RecalculateTransform();
}

const float3& ComponentTransform::GetLocalPosition() const
{
	return local_position;
}

const float3& ComponentTransform::GetGlobalPosition() const
{
	float3 pos, scale;
	Quat rot;

	global_transformation.Decompose(pos, rot, scale);

	return pos;
}

void ComponentTransform::SetLocalScale(const float& x, const float& y, const float& z)
{
	local_scale.x = x;
	local_scale.y = y;
	local_scale.z = z;

	LookScale();

	RecalculateTransform();
}

void ComponentTransform::LookScale()
{
	bool aux_scale = is_scale_negative;

	if (local_scale.x < 0 || local_scale.y < 0 || local_scale.z < 0) {
		is_scale_negative = true;
	}
	else is_scale_negative = false;

	if (aux_scale != is_scale_negative)
		game_object_attached->ScaleNegative(is_scale_negative);
}

const float3& ComponentTransform::GetLocalScale() const
{
	return local_scale;
}

const float3& ComponentTransform::GetGlobalScale() const
{
	float3 pos, scale;
	Quat rot;

	global_transformation.Decompose(pos, rot, scale);

	return scale;
}

void ComponentTransform::SetLocalRotation(const float& x, const float& y, const float& z, const float& angle)
{
	local_rotation.x = x;
	local_rotation.y = y;
	local_rotation.z = z;
	local_rotation.w = angle;

	euler_rotation = local_rotation.ToEulerXYZ();
	euler_rotation.x = RadToDeg(euler_rotation.x);
	euler_rotation.y = RadToDeg(euler_rotation.y);
	euler_rotation.z = RadToDeg(euler_rotation.z);

	RecalculateTransform();
}



const Quat& ComponentTransform::GetLocalRotation() const
{
	return local_rotation;
}

const Quat& ComponentTransform::GetGlobalRotation() const
{
	float3 pos, scale;
	Quat rot;

	global_transformation.Decompose(pos, rot, scale);

	return rot;
}

void ComponentTransform::RecalculateTransform()
{	
	local_transformation = float4x4::FromTRS(local_position, local_rotation, local_scale);

	if (game_object_attached == nullptr)
		return;

	if (game_object_attached->parent != nullptr) {
		ComponentTransform* tr = (ComponentTransform*)game_object_attached->parent->GetComponent(ComponentType::TRANSFORM);
		if (tr != nullptr)
			global_transformation = tr->global_transformation * local_transformation;
		else
			global_transformation = local_transformation;
	}
	else {
		global_transformation = local_transformation;
	}

	std::vector<GameObject*>::iterator item = game_object_attached->children.begin();
	for (; item != game_object_attached->children.end(); ++item) {
		if (*item != nullptr) {
			ComponentTransform* tr = (ComponentTransform*)(*item)->GetComponent(ComponentType::TRANSFORM);
			if (tr != nullptr) tr->RecalculateTransform();
		}
	}

	ComponentMesh* mesh = (ComponentMesh*)game_object_attached->GetComponent(ComponentType::MESH);
	if (mesh != nullptr)
	{
		mesh->RecalculateAABB_OBB();
	}
}


bool ComponentTransform::DrawInspector()
{
	ImGui::Spacing();

	if (ImGui::Checkbox("Enabled", &game_object_attached->enabled))
		game_object_attached->SayChildrenParentIsEnabled(game_object_attached->enabled);
	ImGui::SameLine();

	static char name[30];
	memcpy(name, game_object_attached->GetName(), 30);

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.5F);

	if (ImGui::InputText("##ObjectName", name, 30, ImGuiInputTextFlags_AutoSelectAll)) {
		game_object_attached->SetName(name);
	}


	ImGui::SameLine();

	if (ImGui::Checkbox("Static", &game_object_attached->is_static)) {		
		if (!game_object_attached->is_static && (game_object_attached->children.empty() || !game_object_attached->HasChildrenStatic())) {
			App->objects->octree.Remove(game_object_attached);
		}
		else if (game_object_attached->is_static && game_object_attached->parent != nullptr && game_object_attached->parent->is_static && game_object_attached->children.empty()) {
			App->objects->octree.Insert(game_object_attached, false);
		}
		else {
			popup_static = true;
		}
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	if (game_object_attached->IsPrefab() && !App->objects->prefab_scene)
	{
		if (ImGui::CollapsingHeader("Prefab Options", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("Open Prefab"))
			{
				ResourcePrefab* prefab = (ResourcePrefab*)App->resources->GetResourceWithID(game_object_attached->GetPrefabID());
				if (prefab != nullptr) {
					prefab->OpenPrefabScene();
					return false;
				}
			}

			ImGui::SameLine();
			ImGui::Text("|");
			ImGui::SameLine();

			if (ImGui::Button("Unpack Prefab"))
			{
				game_object_attached->FindPrefabRoot()->UnpackPrefab();
			}

			ImGui::SameLine();
			ImGui::Text("|");
			ImGui::SameLine();

			if (ImGui::Checkbox("Prefab Locked", &game_object_attached->prefab_locked))
			{
				game_object_attached->FindPrefabRoot()->LockPrefab(game_object_attached->prefab_locked);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.1f);
			if (ImGui::Button("Select Prefab Root"))
			{
				App->objects->SetNewSelectedObject(game_object_attached->FindPrefabRoot());
				App->camera->Focus();
				return false;
			}

			ImGui::SameLine();
			ImGui::Text("|");
			ImGui::SameLine();

			if (ImGui::Button("Select Prefab Asset"))
			{
				ResourcePrefab* prefab = (ResourcePrefab*)App->resources->GetResourceWithID(game_object_attached->GetPrefabID());
				if (prefab != nullptr)
					App->ui->panel_project->SelectFile(prefab->GetAssetsPath(), App->resources->assets);
			}

			ImGui::Spacing();
			//ImGui::Separator();
			ImGui::Spacing();

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.06f);
			if (ImGui::Button("Set as the Original "))
			{
				GameObject* obj =game_object_attached->FindPrefabRoot();
				if (obj != nullptr) {
					std::vector<GameObject*>::iterator item = obj->parent->children.begin();
					for (; item != obj->parent->children.end(); ++item) {
						if (*item != nullptr && *item == obj) {
							ResourcePrefab* prefab = (ResourcePrefab*)App->resources->GetResourceWithID(obj->GetPrefabID());
							if (prefab != nullptr) {
								(*item)->ToDelete();
								prefab->ConvertToGameObjects(obj->parent, item - obj->parent->children.begin(), local_position);
							}
							return false;
						}
					}
				}
			}

			ImGui::SameLine();
			ImGui::Text("|");
			ImGui::SameLine();

			/*ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.25);*/
			if (ImGui::Button("Save as the Original"))
			{
				ResourcePrefab* prefab = (ResourcePrefab*)App->resources->GetResourceWithID(game_object_attached->GetPrefabID());
				if (prefab != nullptr) {
					prefab->Save(game_object_attached->FindPrefabRoot());
					App->objects->SetNewSelectedObject(game_object_attached);
					return false;
				}
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
		}
	}

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {

		RightClickMenu("Transform");
		static bool set_cntrl_Z = true;
		ImGui::Spacing();
		ImGui::Text("Position  ");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(70);
		ImGui::PushID(1);
		static float3 view_pos;
		view_pos = local_position;
		if (ImGui::DragFloat("X", &view_pos.x, 0.5F,0,0,"%.3f",1, game_object_attached->is_static)) {
			if (set_cntrl_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			set_cntrl_Z = false;
			local_position = view_pos;
			RecalculateTransform();
		}
		else if (!set_cntrl_Z && ImGui::IsMouseReleased(0)) {
			set_cntrl_Z = true;
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::SetNextItemWidth(70);
		ImGui::PushID(2);
		if (ImGui::DragFloat("Y", &view_pos.y, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static)) {
			if (set_cntrl_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			set_cntrl_Z = false;
			local_position = view_pos;
			RecalculateTransform();
		}
		else if (!set_cntrl_Z && ImGui::IsMouseReleased(0)) {
			set_cntrl_Z = true;
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::SetNextItemWidth(70);
		ImGui::PushID(3);
		if (ImGui::DragFloat("Z", &view_pos.z, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static)) {
			if (set_cntrl_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			set_cntrl_Z = false;
			local_position = view_pos;
			RecalculateTransform();
		}
		else if (!set_cntrl_Z && ImGui::IsMouseReleased(0)) {
			set_cntrl_Z = true;
		}
		ImGui::PopID();
		ImGui::Spacing();

		ImGui::Text("Rotation  ");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(70);
		ImGui::PushID(4);
		static float3 view_rot;
		view_rot = euler_rotation;
		if (ImGui::DragFloat("X", &view_rot.x, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static)) {
			if (set_cntrl_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			set_cntrl_Z = false;
			euler_rotation = view_rot;
			float3 aux = euler_rotation;
			aux.x = DegToRad(euler_rotation.x);
			aux.y = DegToRad(euler_rotation.y);
			aux.z = DegToRad(euler_rotation.z);
			local_rotation = Quat::FromEulerXYZ(aux.x, aux.y, aux.z);
			RecalculateTransform();
		}
		else if (!set_cntrl_Z && ImGui::IsMouseReleased(0)) {
			set_cntrl_Z = true;
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::SetNextItemWidth(70);
		ImGui::PushID(5);
		if (ImGui::DragFloat("Y", &view_rot.y, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static)) {
			if (set_cntrl_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			set_cntrl_Z = false;
			euler_rotation = view_rot;
			float3 aux = euler_rotation;
			aux.x = DegToRad(euler_rotation.x);
			aux.y = DegToRad(euler_rotation.y);
			aux.z = DegToRad(euler_rotation.z);
			local_rotation = Quat::FromEulerXYZ(aux.x, aux.y, aux.z);
			RecalculateTransform();
		}
		else if (!set_cntrl_Z && ImGui::IsMouseReleased(0)) {
			set_cntrl_Z = true;
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::SetNextItemWidth(70);
		ImGui::PushID(6);
		if (ImGui::DragFloat("Z", &view_rot.z, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static)) {
			if (set_cntrl_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			set_cntrl_Z = false;
			euler_rotation = view_rot;
			float3 aux = euler_rotation;
			aux.x = DegToRad(euler_rotation.x);
			aux.y = DegToRad(euler_rotation.y);
			aux.z = DegToRad(euler_rotation.z);
			local_rotation = Quat::FromEulerXYZ(aux.x, aux.y, aux.z);
			RecalculateTransform();
		}
		else if (!set_cntrl_Z && ImGui::IsMouseReleased(0)) {
			set_cntrl_Z = true;
		}
		ImGui::PopID();
		ImGui::Spacing();

		ImGui::Text("Scale     ");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(70);
		ImGui::PushID(7);
		static float3 view_scale;
		view_scale = local_scale;
		if (ImGui::DragFloat("X", &view_scale.x, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static)) {
			if (set_cntrl_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			set_cntrl_Z = false;
			local_scale = view_scale;
			LookScale();
			RecalculateTransform();
		}
		else if (!set_cntrl_Z && ImGui::IsMouseReleased(0)) {
			set_cntrl_Z = true;
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::SetNextItemWidth(70);
		ImGui::PushID(8);
		if (ImGui::DragFloat("Y", &view_scale.y, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static)) {
			if (set_cntrl_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			set_cntrl_Z = false;
			local_scale = view_scale;
			LookScale();
			RecalculateTransform();
		}
		else if (!set_cntrl_Z && ImGui::IsMouseReleased(0)) {
			set_cntrl_Z = true;
		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::SetNextItemWidth(70);
		ImGui::PushID(9);
		if (ImGui::DragFloat("Z", &view_scale.z, 0.5F, 0, 0, "%.3f", 1, game_object_attached->is_static)) {
			if (set_cntrl_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			set_cntrl_Z = false;
			local_scale = view_scale;
			LookScale();
			RecalculateTransform();
		}
		else if (!set_cntrl_Z && ImGui::IsMouseReleased(0)) {
			set_cntrl_Z = true;
		}
		ImGui::PopID();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}
	else {
		RightClickMenu("Transform");
	}

	if (popup_static) {
		if (game_object_attached->is_static) {
			if (game_object_attached->parent != nullptr && !game_object_attached->parent->is_static) {
				// if your parent is dynamic, you cant be static
				ImGui::OpenPopup("Static Problems!");
				ImGui::SetNextWindowSize({ 290,140 });
				if (ImGui::BeginPopupModal("Static Problems!", &popup_static, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
				{
					ImGui::Spacing();

					ImGui::Text("Parent of selected object isn't static");
					ImGui::Spacing();
					ImGui::Text("GameObject: %s", game_object_attached->GetName());
					ImGui::Spacing();
					ImGui::Text("Parent: %s", game_object_attached->parent->GetName());

					ImGui::Spacing();

					ImGui::Text("Make parent static to set this static");
					ImGui::SetCursorPosX(((ImGui::GetWindowWidth()) * 0.5f) - 50);

					if (ImGui::Button("Accept", { 100,20 })) {
						popup_static = false;
						game_object_attached->is_static = false;
					}
					ImGui::EndPopup();
				}
				else {
					popup_static = false;
					game_object_attached->is_static = false;
				}
			}
			else {
				ImGui::OpenPopup("Static Question");
				ImGui::SetNextWindowSize({ 270,120 });
				if (ImGui::BeginPopupModal("Static Question", &popup_static, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
				{
					ImGui::Spacing();
					ImGui::SetCursorPosX(20);
					ImGui::Text("Do you want to make all children\nstatic as well?");
					ImGui::Spacing();

					ImGui::SetCursorPosX(12);
					if (ImGui::Button("Change children")) {
						game_object_attached->ChangeStatic(game_object_attached->is_static);
						App->objects->octree.Insert(game_object_attached, true);
						popup_static = false;
					}
					ImGui::SameLine();
					if (ImGui::Button("Only this object")) {
						App->objects->octree.Insert(game_object_attached, false);
						popup_static = false;
					}
					ImGui::Spacing();
					ImGui::SetCursorPosX(((ImGui::GetWindowWidth()) * 0.5f) - 35);
					if (ImGui::Button("Cancel")) {
						game_object_attached->is_static = false;
						popup_static = false;
					}

					ImGui::EndPopup();
				}
				else {
					game_object_attached->is_static = false;
					popup_static = false;
				}
			}
		}
		else {
			// if you make a object dynamic, children will transform to dynamic

			// TODO: popup if you make x object dynamic, all children will become dynamic too. Do you want this? 
			ImGui::OpenPopup("Static Question");
			ImGui::SetNextWindowSize({ 290,150 });
			if (ImGui::BeginPopupModal("Static Question", &popup_static, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
			{
				ImGui::Spacing();
				ImGui::SetCursorPosX(20);
				ImGui::Text("To set this object dynamic, you must\nset children dynamic too");
				ImGui::SetCursorPosX(20);
				ImGui::Text("Do you want to make all children\ndynamic as well?");
				ImGui::Spacing();
				ImGui::SetCursorPosX(20);
				ImGui::Text("GameObject: %s", game_object_attached->GetName());

				ImGui::Spacing();

				ImGui::SetCursorPosX(37);

				if (ImGui::Button("Yes, change children")) {
					popup_static = false;
					game_object_attached->ChangeStatic(game_object_attached->is_static);
					App->objects->octree.Remove(game_object_attached);
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel")) {
					popup_static = false;
					game_object_attached->is_static = true;
				}
				ImGui::EndPopup();
			}
			else {
				popup_static = false;
				game_object_attached->is_static = true;
			}
		}
	}
	return true;
}

void ComponentTransform::SetScaleNegative(const bool& negative)
{
	is_scale_negative = negative;
}

bool ComponentTransform::IsScaleNegative()
{
	return is_scale_negative;
}

void ComponentTransform::Reset()
{
	local_scale = { 1,1,1 };
	local_position = { 0,0,0 };
	local_rotation = { 0,0,0,0 };
	
	euler_rotation = local_rotation.ToEulerXYZ();
	euler_rotation.x = RadToDeg(euler_rotation.x);
	euler_rotation.y = RadToDeg(euler_rotation.y);
	euler_rotation.z = RadToDeg(euler_rotation.z);

	is_scale_negative = false;

	RecalculateTransform();
}

void ComponentTransform::SetComponent(Component* component)
{
	if (component->GetType() == type) {

		ComponentTransform* transform = (ComponentTransform*)component;

		local_position = transform->local_position;
		local_scale = transform->local_scale;
		local_rotation = transform->local_rotation;
		euler_rotation = transform->euler_rotation;

		LookScale();
		RecalculateTransform();
	}
}

void ComponentTransform::SaveComponent(JSONArraypack* to_save)
{
	to_save->SetNumber("Type", (int)type);
	to_save->SetFloat3("Position", local_position);
	to_save->SetQuat("Rotation", local_rotation);
	to_save->SetFloat3("Scale", local_scale);
	to_save->SetBoolean("ScaleNegative", is_scale_negative);
	to_save->SetString("ID", std::to_string(ID));
}

void ComponentTransform::LoadComponent(JSONArraypack* to_load)
{
	local_position = to_load->GetFloat3("Position");
	local_rotation = to_load->GetQuat("Rotation");
	local_scale = to_load->GetFloat3("Scale");
	is_scale_negative = to_load->GetBoolean("ScaleNegative");
	ID = std::stoull(to_load->GetString("ID"));
	euler_rotation = local_rotation.ToEulerXYZ();
	euler_rotation.x = RadToDeg(euler_rotation.x);
	euler_rotation.y = RadToDeg(euler_rotation.y);
	euler_rotation.z = RadToDeg(euler_rotation.z);

	local_transformation = float4x4::FromTRS(local_position, local_rotation, local_scale);

	if (game_object_attached->parent != nullptr) {
		ComponentTransform* tr = (ComponentTransform*)game_object_attached->parent->GetComponent(ComponentType::TRANSFORM);
		if (tr != nullptr) global_transformation = tr->global_transformation * local_transformation;
		else global_transformation = local_transformation;
	}
	else
		global_transformation = local_transformation;
}

void ComponentTransform::SetLocalTransform(float4x4 &transform_matrix)
{
	local_transformation = transform_matrix;
	local_transformation.Decompose(local_position, local_rotation, local_scale);
	RecalculateTransform();
}

void ComponentTransform::SetGlobalTransformation(float4x4 global_transformation)
{
	float3 position, scale;
	Quat rotation;
	global_transformation.Decompose(position, rotation, scale);
	
	if (App->ui->panel_scene->guizmo_operation == ImGuizmo::OPERATION::SCALE) { // why... i would like to know it...
		this->local_scale = scale;
	}
	else {
		this->local_position = position;
		this->local_rotation = rotation;
		euler_rotation = local_rotation.ToEulerXYZ();
		euler_rotation.x = RadToDeg(euler_rotation.x);
		euler_rotation.y = RadToDeg(euler_rotation.y);
		euler_rotation.z = RadToDeg(euler_rotation.z);
	}

	RecalculateTransform();
}