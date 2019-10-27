#include "ComponentMesh.h"
#include "glew/include/glew.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ComponentMaterial.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Color.h"
#include "ResourceMesh.h"

ComponentMesh::ComponentMesh(GameObject* attach) : Component(attach)
{
	type = ComponentType::MESH;
}

ComponentMesh::~ComponentMesh()
{
	if (game_object_attached != nullptr && game_object_attached->HasComponent(ComponentType::MATERIAL))
	{
		static_cast<ComponentMaterial*>(game_object_attached->GetComponent(ComponentType::MATERIAL))->not_destroy = false;
	}
}

void ComponentMesh::DrawPolygon()
{
	if (mesh == nullptr || mesh->id_index <= 0)
		return;

	if (game_object_attached->IsSelected() || game_object_attached->IsParentSelected()) {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);

	if (transform->IsScaleNegative())
		glFrontFace(GL_CW);

	glPushMatrix();
	glMultMatrixf(transform->global_transformation.Transposed().ptr());

	glEnableClientState(GL_VERTEX_ARRAY);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 0.1f);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	if (mesh->uv_cords != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uv);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	if (mesh->normals != nullptr) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glNormalPointer(GL_FLOAT, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glDrawElements(GL_TRIANGLES, mesh->num_index * 3, GL_UNSIGNED_INT, 0);

	if (transform->IsScaleNegative())
		glFrontFace(GL_CCW);

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
	if (mesh == nullptr || mesh->id_index <= 0)
		return;


	if (!glIsEnabled(GL_STENCIL_TEST))
		return;
	if (game_object_attached->IsParentSelected())
	{
		glColor3f(App->objects->parent_outline_color.r, App->objects->parent_outline_color.g, App->objects->parent_outline_color.b);
		glLineWidth(App->objects->parent_line_width);
	}
	else
	{
		glColor3f(App->objects->no_child_outline_color.r, App->objects->no_child_outline_color.g, App->objects->no_child_outline_color.b);
		glLineWidth(App->objects->no_child_line_width);
	}

	glStencilFunc(GL_NOTEQUAL, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	glPolygonMode(GL_FRONT, GL_LINE);

	glPushMatrix();
	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
	glMultMatrixf(transform->global_transformation.Transposed().ptr());

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawElements(GL_TRIANGLES, mesh->num_index * 3, GL_UNSIGNED_INT, 0);

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisableClientState(GL_VERTEX_ARRAY);
	glLineWidth(1);

	glPopMatrix();
}

void ComponentMesh::DrawMesh()
{
	if (mesh == nullptr || mesh->id_index <= 0)
		return;

	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);

	glPushMatrix();
	glMultMatrixf(transform->global_transformation.Transposed().ptr());

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glLineWidth(App->objects->mesh_line_width);
	glColor3f(App->objects->mesh_color.r, App->objects->mesh_color.g, App->objects->mesh_color.b);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, mesh->num_index * 3, GL_UNSIGNED_INT, NULL);

	glLineWidth(1);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();

}

void ComponentMesh::DrawVertexNormals()
{
	if (mesh == nullptr || mesh->id_index <= 0)
		return;

	if (mesh->normals != nullptr) {
		ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);

		glPushMatrix();
		glMultMatrixf(transform->global_transformation.Transposed().ptr());

		glColor3f(App->objects->vertex_n_color.r, App->objects->vertex_n_color.g, App->objects->vertex_n_color.b);
		glLineWidth(App->objects->vertex_n_width);
		glBegin(GL_LINES);
		for (uint i = 0; i < mesh->num_vertex * 3; i += 3)
		{
			glVertex3f(mesh->vertex[i], mesh->vertex[i + 1], mesh->vertex[i + 2]);
			glVertex3f(mesh->vertex[i] + mesh->normals[i] * App->objects->vertex_normal_length, mesh->vertex[i + 1] + mesh->normals[i + 1] * App->objects->vertex_normal_length, mesh->vertex[i + 2] + mesh->normals[i + 2] * App->objects->vertex_normal_length);
		}
		glEnd();
		glLineWidth(1);

		glPopMatrix();
	}
}

void ComponentMesh::DrawFaceNormals()
{
	if (mesh == nullptr || mesh->id_index <= 0)
		return;

	if (mesh->normals != nullptr) {
		ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);

		glPushMatrix();
		glMultMatrixf(transform->global_transformation.Transposed().ptr());

		glColor3f(App->objects->face_n_color.r, App->objects->face_n_color.g, App->objects->face_n_color.b);
		glLineWidth(App->objects->face_n_width);
		glBegin(GL_LINES);
		for (uint i = 0; i < mesh->num_index; i += 3)
		{
			glVertex3f(mesh->center_point[i], mesh->center_point[i + 1], mesh->center_point[i + 2]);
			glVertex3f(mesh->center_point[i] + mesh->center_point_normal[i] * App->objects->face_normal_length, mesh->center_point[i + 1] + mesh->center_point_normal[i+ 1] * App->objects->face_normal_length, mesh->center_point[i + 2] + mesh->center_point_normal[i + 2] * App->objects->face_normal_length);
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

	if (ImGui::CollapsingHeader("Mesh", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		RightClickMenu("Mesh");

		ImGui::Spacing();

		ImGui::Text("Geometry Information");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Vertex count:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", (mesh != nullptr ? mesh->num_vertex : 0));
		ImGui::Text("Triangles count:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", (mesh != nullptr ? mesh->num_faces : 0));

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		
		ImGui::Checkbox("Active Mesh          ", &view_mesh); ImGui::SameLine(); ImGui::Checkbox("Active Wireframe", &wireframe);
		ImGui::Checkbox("Active Vertex Normals", &view_vertex_normals);	ImGui::SameLine();ImGui::Checkbox("Active Face Normals", &view_face_normals);
		ImGui::Checkbox("Draw AABB", &draw_AABB);
		ImGui::Spacing();

		
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}
	else
		RightClickMenu("Mesh");
}

void ComponentMesh::DrawGlobalAABB()
{
	glColor3f(App->objects->global_AABB_color.r, App->objects->global_AABB_color.g, App->objects->global_AABB_color.b);
	glLineWidth(App->objects->AABB_line_width);
	glBegin(GL_LINES);

	glVertex3f(global_aabb.minPoint.x, global_aabb.minPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.minPoint.y, global_aabb.minPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.minPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.minPoint.x, global_aabb.minPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.minPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.minPoint.x, global_aabb.maxPoint.y, global_aabb.minPoint.z);


	glVertex3f(global_aabb.maxPoint.x, global_aabb.minPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.maxPoint.y, global_aabb.minPoint.z);

	glVertex3f(global_aabb.maxPoint.x, global_aabb.minPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.minPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.maxPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.minPoint.x, global_aabb.maxPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.maxPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.maxPoint.y, global_aabb.minPoint.z);

	glVertex3f(global_aabb.maxPoint.x, global_aabb.maxPoint.y, global_aabb.minPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.maxPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.maxPoint.x, global_aabb.minPoint.y, global_aabb.maxPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.maxPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.minPoint.y, global_aabb.maxPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.minPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.minPoint.y, global_aabb.maxPoint.z);
	glVertex3f(global_aabb.minPoint.x, global_aabb.maxPoint.y, global_aabb.maxPoint.z);

	glVertex3f(global_aabb.minPoint.x, global_aabb.maxPoint.y, global_aabb.maxPoint.z);
	glVertex3f(global_aabb.maxPoint.x, global_aabb.maxPoint.y, global_aabb.maxPoint.z);
	
	glLineWidth(1);
	glEnd();
}

void ComponentMesh::Reset()
{
	view_mesh = false;
	wireframe = false;
	view_vertex_normals = false;
	view_face_normals = false;
	draw_AABB = true;
	draw_OBB = true;
}

void ComponentMesh::SetComponent(Component* component)
{
	if (component->GetType() == type) {

		ComponentMesh* tmp = (ComponentMesh*)component;

		mesh = tmp->mesh;

		view_mesh = tmp->view_mesh;
		wireframe = tmp->wireframe;
		view_vertex_normals = tmp->view_vertex_normals;
		view_face_normals = tmp->view_face_normals;

	}
}

AABB ComponentMesh::GenerateAABB()
{
	local_aabb.SetNegativeInfinity();
	local_aabb.Enclose((float3*)mesh->vertex, mesh->num_vertex);

	return local_aabb;
}

AABB ComponentMesh::GetGlobalAABB()
{
	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
	obb = GenerateAABB();
	obb.Transform(transform->global_transformation);

	global_aabb.SetNegativeInfinity();
	global_aabb.Enclose(obb);
	
	return global_aabb;
}