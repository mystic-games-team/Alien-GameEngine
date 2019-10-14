#include "ComponentMaterial.h"
#include "glew/include/glew.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleImporter.h"

ComponentMaterial::ComponentMaterial(GameObject* attach) : Component(attach)
{
	type = ComponentType::MATERIAL;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::BindTexture()
{
	if (texture != nullptr && texture->id > 0) {
		// enable textures
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, texture->id);
	}
	glColor3f(color.r, color.g, color.b);
}

void ComponentMaterial::DrawInspector()
{
	ImGui::PushID(this);
	ImGui::Checkbox("##CmpActive", &enabled);
	ImGui::PopID();
	ImGui::SameLine();
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) 
	{
		ImGui::Spacing();

		ImGui::Spacing();
		ImGui::ColorEdit3("Material Color", &color, ImGuiColorEditFlags_Float);
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
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text("Texture Size:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", texture->width);
			ImGui::SameLine(); ImGui::Text("x"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", texture->height);
			ImGui::Text("Path:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%s", texture->path);

			ImGui::Image((ImTextureID)texture->id, { ImGui::GetWindowWidth() ,ImGui::GetWindowWidth() });
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
		}
		else {
			ImGui::SameLine(220, 15);
			if (ImGui::Button("Add Texture", { 120,20 })) {
				change_texture_menu = true;
			}
		}

		if (change_texture_menu) {

			ImGui::OpenPopup("Textures Loaded");
			ImGui::SetNextWindowSize({ 500,350 });
			if (ImGui::BeginPopupModal("Textures Loaded", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
				if (ImGui::BeginChild("", { 470,285 }, true, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
				
					ImGui::Columns(3, 0, false);
					ImGui::SetColumnWidth(0, 156);
					ImGui::SetColumnWidth(1, 156);
					ImGui::SetColumnWidth(2, 156);
					
					std::vector<Texture*>::iterator item = App->importer->textures.begin();
					for (; item != App->importer->textures.end(); ++item) {
						if (*item != nullptr) {
							ImGui::Image((ImTextureID)(*item)->id, { 156,156 });
							ImGui::NextColumn();
						}
					}

					ImGui::EndChild();
				}
				ImGui::Spacing();
				ImGui::Text("");
				ImGui::SameLine(355);
				if (ImGui::Button("Apply", { 120,20 })) {
					change_texture_menu = false;
				}
				ImGui::SameLine(215);
				if (ImGui::Button("Cancel", { 120,20 })) {
					change_texture_menu = false;
				}
				ImGui::EndPopup();
			}
		}

	}
}
