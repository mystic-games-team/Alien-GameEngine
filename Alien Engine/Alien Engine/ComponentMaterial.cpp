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
	glDeleteBuffers(1, &id_texture);
}

void ComponentMaterial::BindTexture()
{
	if (id_texture > 0) {
		// enable textures
		ComponentMesh* mesh = (ComponentMesh*)game_object_attached->GetComponent(ComponentType::MESH);

		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, id_texture);
		// set UV
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uv);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		
		glColor3f(color.r, color.g, color.b);
	}
}

void ComponentMaterial::DrawInspector()
{
	ImGui::PushID(this);
	ImGui::Checkbox("##CmpActive", &enabled);
	ImGui::PopID();
	ImGui::SameLine();
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {


		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}


}
