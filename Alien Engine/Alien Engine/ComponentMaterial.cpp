#include "ComponentMaterial.h"
#include "glew/include/glew.h"
#include "GameObject.h"
#include "imgui/imgui.h"
#include "ComponentMesh.h"

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
		ImGui::Text("Material Information");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Spacing();
		ImGui::ColorEdit3("Material Color", &color, ImGuiColorEditFlags_Float);
		ImGui::SliderFloat("Object Alpha", &color.a, 0, 1);
		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Spacing();

		if (texture != nullptr)
		{
			ImGui::Spacing();
			ImGui::Text("Texture Information");

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
	}
}
