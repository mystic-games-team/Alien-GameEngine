#include "ComponentMaterial.h"
#include "glew/include/glew.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "ComponentMesh.h"
#include "Application.h"
#include "ResourceTexture.h"

ComponentMaterial::ComponentMaterial(GameObject* attach) : Component(attach)
{
	type = ComponentType::MATERIAL;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::BindTexture()
{

	if (texture != nullptr && texture->id > 0 && texture_activated) {
		// enable textures
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, texture->id);
	}
	glColor4f(color.r, color.g, color.b, color.a);
	
}

void ComponentMaterial::DrawInspector()
{
	ImGui::PushID(this);
	ImGui::Checkbox("##CmpActive", &enabled);
	ImGui::PopID();
	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Material", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		RightClickMenu("Material");

		ImGui::Spacing();

		ImGui::Spacing();
		ImGui::ColorEdit3("Material Color", &color, ImGuiColorEditFlags_Float);
		ImGui::SliderFloat("Alpha", &color.a, 0.0F, 1.0F);
		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Spacing();


		ImGui::Spacing();
		ImGui::Text("Texture Information");

		if (texture != nullptr)
		{
			ImGui::SameLine(220, 15);
			if (ImGui::Button("Change Texture", { 120,20 })) {
				change_texture_menu = true;
			}

			ImGui::SameLine(140, 15);
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.5F,0,0,1 });
			if (ImGui::Button("Delete", { 60,20 })) {
				texture = nullptr;
				ImGui::PopStyleColor();
				return;
			}

			ImGui::PopStyleColor();

			ImGui::Checkbox("Texture Active", &texture_activated);

			ImGui::Text("Texture Size:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", texture->width);
			ImGui::SameLine(); ImGui::Text("x"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", texture->height);
			ImGui::Text("Path:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%s", texture->GetAssetsPath());

			ImGui::Image((ImTextureID)texture->id, { ImGui::GetWindowWidth() ,ImGui::GetWindowWidth() });
			ImGui::Spacing();
		}
		else {
			ImGui::SameLine(220, 15);
			if (ImGui::Button("Add Texture", { 120,20 })) {
				change_texture_menu = true;
			}
		}

		if (change_texture_menu) {
			/*_________________________________________________________________*/
			static ResourceTexture* tex = nullptr;
			static bool first = true;
			if (first) { // SAD need to think what to do here, this sucks :D SAD
				if (texture == nullptr && !App->resources->resource_textures.empty())
					tex = App->resources->resource_textures.front(); // checkers
				else tex = texture;
				first = false;
			}
			/*_________________________________________________________________*/

			ImGui::OpenPopup("Textures Loaded");
			ImGui::SetNextWindowSize({ 522,570 });
			if (ImGui::BeginPopupModal("Textures Loaded", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
				ImGui::Spacing();
				ImGui::NewLine();
				ImGui::SameLine(190);
				ImGui::Text("Texture Selected");
				ImGui::Text("");
				ImGui::SameLine(170);
				if (tex != nullptr) {
					ImGui::Image((ImTextureID)tex->id, { 150,150 });
					ImGui::Spacing();
					ImGui::Text("");
					ImGui::SameLine(150);
					ImGui::Text("Texture Size:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", tex->width);
					ImGui::SameLine(); ImGui::Text("x"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", tex->height);
					ImGui::Text("");
					ImGui::SameLine(112);
					ImGui::Text("Path:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%s", tex->GetAssetsPath());
				}
				ImGui::Spacing();
				if (ImGui::BeginChild("##TexturesSelectorChild", { 492,285 }, true, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {

					ImGui::Columns(3, 0, false);
					ImGui::SetColumnWidth(0, 156);
					ImGui::SetColumnWidth(1, 156);
					ImGui::SetColumnWidth(2, 156);
					
					std::vector<ResourceTexture*>::iterator item = App->resources->resource_textures.begin();
					for (; item != App->resources->resource_textures.end(); ++item) {
						if (*item != nullptr && (*item)->is_custom) {
							ImGui::ImageButton((ImTextureID)(*item)->id, { 140,140 });
							if (ImGui::IsItemClicked()) {
								tex = (*item);
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
					texture = tex;
					change_texture_menu = false;
				}
				ImGui::SameLine(237);
				if (ImGui::Button("Cancel", { 120,20 })) {
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

}

void ComponentMaterial::Reset()
{
	color = { 1,1,1,1 };
	texture = nullptr;
}

void ComponentMaterial::SetComponent(Component* component)
{
	if (component->GetType() == type) {

		ComponentMaterial* material = (ComponentMaterial*)component;

		texture = material->texture;
		material_index = material->material_index;
		color = material->color;
	}
}
