#include "ComponentMesh.h"
#include "glew/include/glew.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ComponentMaterial.h"
#include "MathGeoLib/include/MathGeoLib.h"

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


	if (game_object_attached != nullptr && game_object_attached->HasComponent(ComponentType::MATERIAL))
	{
		static_cast<ComponentMaterial*>(game_object_attached->GetComponent(ComponentType::MATERIAL))->not_destroy = false;
	}
}

void ComponentMesh::DrawPolygon()
{
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

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	if (uv_cords != nullptr) {
		glBindBuffer(GL_ARRAY_BUFFER, id_uv);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	if (normals != nullptr) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glNormalPointer(GL_FLOAT, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, 0);

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

	if (ImGui::CollapsingHeader("Mesh", &not_destroy, ImGuiTreeNodeFlags_DefaultOpen))
	{
		RightClickMenu("Mesh");

		ImGui::Spacing();

		ImGui::Text("Geometry Information");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Vertex count:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", num_vertex);
		ImGui::Text("Triangles count:"); ImGui::SameLine(); ImGui::TextColored({ 255, 216, 0, 100 }, "%i", num_faces);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		
		ImGui::Checkbox("Active Mesh          ", &view_mesh);
		ImGui::SameLine();
		ImGui::Checkbox("Active Wireframe", &wireframe);
		ImGui::Checkbox("Active Vertex Normals", &view_vertex_normals);
		ImGui::SameLine();
		ImGui::Checkbox("Active Face Normals", &view_face_normals);
		ImGui::Spacing();

		
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}
	else
		RightClickMenu("Mesh");
}

void ComponentMesh::Reset()
{
	view_mesh = false;
	wireframe = false;
	view_vertex_normals = false;
	view_face_normals = false;
}

void ComponentMesh::SetComponent(Component* component)
{
	if (component->GetType() == type) {

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

		ComponentMesh* mesh = (ComponentMesh*)component;

		num_index = mesh->num_index;
		num_vertex = mesh->num_vertex;
		num_faces = mesh->num_faces;
		
		if (mesh->index != nullptr) {
			index = new uint[num_index];
			memcpy(index, mesh->index, sizeof(uint) * num_index);
		}
		if (mesh->vertex != nullptr) {
			vertex = new float[num_vertex * 3];
			memcpy(vertex, mesh->vertex, sizeof(float) * num_vertex * 3);
		}
		if (mesh->normals != nullptr) {
			normals = new float[num_vertex * 3];
			center_point = new float[num_faces * 3];
			center_point_normal = new float[num_faces * 3];
			memcpy(normals, mesh->normals, sizeof(float) * num_vertex * 3);
			memcpy(center_point, mesh->center_point, sizeof(float) * num_faces * 3);
			memcpy(center_point_normal, mesh->center_point_normal, sizeof(float) * num_faces * 3);
		}
		if (mesh->uv_cords != nullptr) {
			uv_cords = new float[num_vertex * 3];
			memcpy(uv_cords, mesh->uv_cords, sizeof(float) * num_vertex * 3);
		}


		glGenBuffers(1, &id_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertex * 3,
			&mesh->vertex[0], GL_STATIC_DRAW);

		// index
		glGenBuffers(1, &id_index);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_index,
			&index[0], GL_STATIC_DRAW);

		if (uv_cords != nullptr) {
			// UV
			glGenBuffers(1, &id_uv);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_uv);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * num_vertex * 3,
				&uv_cords[0], GL_STATIC_DRAW);
		}

		if (normals != nullptr) {
			// normals
			glGenBuffers(1, &id_normals);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_normals);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * num_vertex * 3,
				&normals[0], GL_STATIC_DRAW);
		}

		view_mesh = mesh->view_mesh;
		wireframe = mesh->wireframe;
		view_vertex_normals = mesh->view_vertex_normals;
		view_face_normals = mesh->view_face_normals;
	}
}

AABB ComponentMesh::GenerateAABB()
{
	local_aabb.SetNegativeInfinity();
	local_aabb.Enclose((float3*)vertex, num_vertex);

	return local_aabb;
}

void ComponentMesh::SetGlobalBoundingBoxes()
{
	ComponentTransform* transform = (ComponentTransform*)game_object_attached->GetComponent(ComponentType::TRANSFORM);
	obb = GenerateAABB();
	obb.Translate(transform->GetGlobalPosition());
	obb.Scale(obb.CenterPoint(), transform->GetGlobalScale());
	obb.Transform(transform->GetGlobalRotation());
	
	global_aabb.SetNegativeInfinity();
	global_aabb.Enclose(obb);
}
