#include "ComponentMaterial.h"
#include "glew/include/glew.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "ComponentMesh.h"
#include "Application.h"
#include "ResourceTexture.h"
#include "ReturnZ.h"

ComponentMaterial::ComponentMaterial(GameObject* attach) : Component(attach)
{
	type = ComponentType::MATERIAL;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::BindTexture()
{
	ComponentMesh* mesh = (ComponentMesh*)game_object_attached->GetComponent(ComponentType::MESH);
	if (texture != nullptr && texture->id > 0 && texture_activated && mesh != nullptr && mesh->mesh != nullptr) {
		// enable textures
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, texture->id);
	}
	glColor4f(color.r, color.g, color.b, color.a);
	
}

bool ComponentMaterial::DrawInspector()
{
	static bool en;
	ImGui::PushID(this);
	en = enabled;
	if (ImGui::Checkbox("##CmpActive", &en)) {
		ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
		enabled = en;
	}
	ImGui::PopID();
	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Material", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		RightClickMenu("Material");

		ImGui::Spacing();
		static bool set_Z = true;
		ImGui::Spacing();
		static Color col;
		col = color;
		if (ImGui::ColorEdit3("Material Color", &col, ImGuiColorEditFlags_Float)) {
			if (set_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			set_Z = false;
			color = col;
		}
		else if (!set_Z && ImGui::IsMouseReleased(0)) {
			set_Z = true;
		}

		if (ImGui::SliderFloat("Alpha", &col.a, 0.0F, 1.0F)) {
			if (set_Z)
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
			set_Z = false;
			color.a = col.a;
		}
		else if (!set_Z && ImGui::IsMouseReleased(0)) {
			set_Z = true;
		}

		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Spacing();
		ImGui::Text("Texture Information");

		static ResourceTexture* selected_texture = nullptr;
		if (texture != nullptr)
		{
			ImGui::SameLine(220, 15);
			if (ImGui::Button("Change Texture", { 120,20 })) {
				/*change_texture_menu = true;
				selected_texture = texture;*/
			}

			ImGui::SameLine(140, 15);
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.65F,0,0,1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.8F,0,0,1 });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0.95F,0,0,1 });
			if (ImGui::Button("Delete", { 60,20 })) {
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
				texture = nullptr;
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				return true;
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			static bool check;
			check = texture_activated;
			if (ImGui::Checkbox("Texture Active", &check)) {
				ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
				texture_activated = check;
			}

			ImGui::Text("Texture Size:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", texture->width);
			ImGui::SameLine(); ImGui::Text("x"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", texture->height);
			ImGui::Text("Path:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%s", texture->GetAssetsPath());

			ImGui::Image((ImTextureID)texture->id, { ImGui::GetWindowWidth() ,ImGui::GetWindowWidth() });
			ImGui::Spacing();
		}
		else {
			ImGui::SameLine(220, 15);
			if (ImGui::Button("Add Texture", { 120,20 })) {
				/*change_texture_menu = true;
				selected_texture = texture;*/
			}
		}

		if (change_texture_menu) {
			ImGui::OpenPopup("Textures Loaded");
			ImGui::SetNextWindowSize({ 522,570 });
			if (ImGui::BeginPopupModal("Textures Loaded", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
				ImGui::Spacing();
				ImGui::NewLine();
				ImGui::SameLine(190);
				ImGui::Text("Texture Selected");
				ImGui::Text("");
				ImGui::SameLine(170);
				if (selected_texture != nullptr) {
					ImGui::Image((ImTextureID)selected_texture->id, { 150,150 });
					ImGui::Spacing();
					ImGui::Text("");
					ImGui::SameLine(150);
					ImGui::Text("Texture Size:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", selected_texture->width);
					ImGui::SameLine(); ImGui::Text("x"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", selected_texture->height);
					ImGui::Text("");
					ImGui::SameLine(112);
					ImGui::Text("Path:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%s", selected_texture->GetAssetsPath());
				}
				ImGui::Spacing();

				if (ImGui::BeginChild("##TexturesSelectorChild", { 492,285 }, true, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {	
					ImGui::Columns(3, 0, false);
					ImGui::SetColumnWidth(0, 156);
					ImGui::SetColumnWidth(1, 156);
					ImGui::SetColumnWidth(2, 156);
					
					std::vector<Resource*>::iterator item = App->resources->resources.begin();
					for (; item != App->resources->resources.end(); ++item) {
						if (*item != nullptr && (*item)->GetType() == ResourceType::RESOURCE_TEXTURE && static_cast<ResourceTexture*>(*item)->is_custom) {
							ImGui::ImageButton((ImTextureID)static_cast<ResourceTexture*>(*item)->id, { 140,140 });
							if (ImGui::IsItemClicked()) {
								selected_texture = static_cast<ResourceTexture*>(*item);
							}
							ImGui::NextColumn();
						}
					}

					ImGui::EndChild();
				}
				ImGui::Spacing();
				ImGui::Text("");
				ImGui::SameLine(377);
				if (ImGui::Button("Apply", { 120,20 })) {
					ReturnZ::AddNewAction(ReturnZ::ReturnActions::CHANGE_COMPONENT, this);
					texture = selected_texture;
					selected_texture = nullptr;
					change_texture_menu = false;
				}
				ImGui::SameLine(237);
				if (ImGui::Button("Cancel", { 120,20 })) {
					selected_texture = nullptr;
					change_texture_menu = false;
				}
				ImGui::EndPopup();
			}
		}
		ImGui::Spacing();
		ImGui::Separator();
	}
	else
		RightClickMenu("Material");

	return true;
}

void ComponentMaterial::Reset()
{
	color = { 1,1,1,1 };
	if (texture != nullptr)
		texture->DecreaseReferences();
	texture = nullptr;
}

void ComponentMaterial::SetComponent(Component* component)
{
	if (component->GetType() == type) {

		ComponentMaterial* material = (ComponentMaterial*)component;
		if (texture != nullptr)
			texture->DecreaseReferences();
		texture = material->texture;
		if (texture != nullptr)
			texture->IncreaseReferences();

		color = material->color;
	}
}

void ComponentMaterial::SaveComponent(JSONArraypack* to_save)
{
	to_save->SetNumber("Type", (int)type);
	to_save->SetColor("Color", color);
	to_save->SetBoolean("TextureEnabled", texture_activated);
	to_save->SetString("ID", std::to_string(ID));
	to_save->SetBoolean("HasTexture", (texture != nullptr) ? true : false);
	if (texture != nullptr) {
		to_save->SetString("TextureID", std::to_string(texture->GetID()));
	}
	to_save->SetBoolean("Enabled", enabled);
}

void ComponentMaterial::LoadComponent(JSONArraypack* to_load)
{
	color = to_load->GetColor("Color");
	texture_activated = to_load->GetBoolean("TextureEnabled");
	enabled = to_load->GetBoolean("Enabled");
	if (to_load->GetBoolean("HasTexture")) {
		u64 ID = std::stoull(to_load->GetString("TextureID"));
		texture = (ResourceTexture*)App->resources->GetResourceWithID(ID);
		if (texture != nullptr)
			texture->IncreaseReferences();
	}
	ID = std::stoull(to_load->GetString("ID"));
}

void ComponentMaterial::SetTexture(ResourceTexture* tex)
{
	if (texture != nullptr)
		texture->DecreaseReferences();
	texture = tex;
	if (texture != nullptr)
		texture->IncreaseReferences();
}

const ResourceTexture* ComponentMaterial::GetTexture() const
{
	return texture;
}
