#include "ComponentMesh.h"
#include "glew/include/glew.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Application.h"

ComponentMesh::ComponentMesh(GameObject* attach) : Component(attach)
{
	type = ComponentType::MESH;
}

ComponentMesh::~ComponentMesh()
{
	glDeleteBuffers(1, &id_index);
	glDeleteBuffers(1, &id_normals);
	glDeleteBuffers(1, &id_vertex);
	glDeleteBuffers(1, &id_uv);

	delete[] vertex;
	delete[] index;
	delete[] normals;
	delete[] center_point;
	delete[] center_point_normal;
	delete[] uv_cords;

	vertex = nullptr;
	index = nullptr;
	center_point = nullptr;
	center_point_normal = nullptr;
	normals = nullptr;
	uv_cords = nullptr;
}

void ComponentMesh::DrawPolygon()
{
	if (game_object_attached->IsSelected() || game_object_attached->IsParentSelected()) {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);

	glPushMatrix();
	glMultMatrixf(transform->global_transformation.Transposed().ptr());

	glEnableClientState(GL_VERTEX_ARRAY);

	if (normals != nullptr) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glNormalPointer(GL_FLOAT, 0, 0);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 0.1f);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, 0);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

}

void ComponentMesh::DrawOutLine()
{
	if (!glIsEnabled(GL_STENCIL_TEST))
		return;

	if (game_object_attached->IsParentSelected())
	{
		glColor3f(0, 1, 1);
	}
	else
	{
		glColor3f(1, 0.5f, 0);
	}

	glStencilFunc(GL_NOTEQUAL, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glLineWidth(outline_width);
	glPolygonMode(GL_FRONT, GL_LINE);

	glPushMatrix();
	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
	glMultMatrixf(transform->global_transformation.Transposed().ptr());

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, 0);

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisableClientState(GL_VERTEX_ARRAY);
	glLineWidth(1);

	glPopMatrix();
}

void ComponentMesh::DrawMesh()
{
	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);

	glPushMatrix();
	glMultMatrixf(transform->global_transformation.Transposed().ptr());

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glLineWidth(App->objects->mesh_line_width);
	glColor3f(App->objects->mesh_color.r, App->objects->mesh_color.g, App->objects->mesh_color.b);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, NULL);

	glLineWidth(1);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();

}

void ComponentMesh::DrawVertexNormals()
{
	if (normals != nullptr) {
		ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);

		glPushMatrix();
		glMultMatrixf(transform->global_transformation.Transposed().ptr());

		glColor3f(App->objects->vertex_n_color.r, App->objects->vertex_n_color.g, App->objects->vertex_n_color.b);
		glLineWidth(App->objects->vertex_n_width);
		glBegin(GL_LINES);
		for (uint i = 0; i < num_vertex * 3; i += 3)
		{
			glVertex3f(vertex[i], vertex[i + 1], vertex[i + 2]);
			glVertex3f(vertex[i] + normals[i] * App->objects->vertex_normal_length, vertex[i + 1] + normals[i + 1] * App->objects->vertex_normal_length, vertex[i + 2] + normals[i + 2] * App->objects->vertex_normal_length);
		}
		glEnd();
		glLineWidth(1);

		glPopMatrix();
	}
}

void ComponentMesh::DrawFaceNormals()
{
	if (normals != nullptr) {
		ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);

		glPushMatrix();
		glMultMatrixf(transform->global_transformation.Transposed().ptr());

		glColor3f(App->objects->face_n_color.r, App->objects->face_n_color.g, App->objects->face_n_color.b);
		glLineWidth(App->objects->face_n_width);
		glBegin(GL_LINES);
		for (uint i = 0; i < num_index; i += 3)
		{
			glVertex3f(center_point[i], center_point[i + 1], center_point[i + 2]);
			glVertex3f(center_point[i] + center_point_normal[i] * App->objects->face_normal_length, center_point[i + 1] + center_point_normal[i+ 1] * App->objects->face_normal_length, center_point[i + 2] + center_point_normal[i + 2] * App->objects->face_normal_length);
		}
		glEnd();
		glLineWidth(1);

		glPopMatrix();
	}
}

void ComponentMesh::DrawInspector()
{
	ImGui::PushID(this);
	ImGui::Checkbox("##CmpActive", &enabled);
	ImGui::PopID();
	ImGui::SameLine();
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
		
		ImGui::Spacing();

		ImGui::Checkbox("Draw Outline", &draw_outline);
		ImGui::SliderInt("Outline Width", (int*)& outline_width, 1, 30);

		ImGui::Checkbox("Active Mesh", &view_mesh);
		ImGui::Checkbox("Active Wireframe", &wireframe);
		ImGui::Checkbox("Active Vertex Normals", &view_vertex_normals);
		ImGui::Checkbox("Active Face Normals", &view_face_normals);
		ImGui::Spacing();

		
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}


}
