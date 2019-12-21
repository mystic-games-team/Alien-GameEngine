#include "Gizmos.h"
#include "Application.h"
#include "ResourceMesh.h"

std::vector<Gizmos::Gizmo> Gizmos::active_gizmos;
bool Gizmos::controller = false;

void Gizmos::DrawCube(const float3& position, const float3& size, const Color& color)
{
	float3 centered_pos = { position.x - size.x / 2 ,  position.y - size.y / 2 ,  position.z - size.z / 2 };
	for (uint i = 0; i < Gizmos::active_gizmos.size(); ++i) {
		if (Gizmos::active_gizmos[i].type == PrimitiveType::CUBE) {
			float4x4 matrix = float4x4::identity();
			matrix = matrix.FromTRS(centered_pos, Quat::identity(), size);
			DrawPoly(Gizmos::active_gizmos[i].mesh, matrix, color);
			Gizmos::active_gizmos[i].controller = controller;
			return;
		}
	}
	ResourceMesh* mesh = App->resources->GetPrimitive(PrimitiveType::CUBE);
	Gizmos::active_gizmos.push_back(Gizmo(mesh, true, PrimitiveType::CUBE));
	float4x4 matrix = float4x4::identity();
	matrix = matrix.FromTRS(centered_pos, Quat::identity(), size);
	DrawPoly(mesh, matrix, color);
}

void Gizmos::DrawWireCube(const float3& position, const float3& size, const Color& color, float line_width)
{
	float3 centered_pos = { position.x - size.x / 2 ,  position.y - size.y / 2 ,  position.z - size.z / 2 };
	for (uint i = 0; i < Gizmos::active_gizmos.size(); ++i) {
		if (Gizmos::active_gizmos[i].type == PrimitiveType::CUBE) {
			float4x4 matrix = float4x4::identity();
			matrix = matrix.FromTRS(centered_pos, Quat::identity(), size);
			DrawWire(Gizmos::active_gizmos[i].mesh, matrix, color, line_width);
			Gizmos::active_gizmos[i].controller = controller;
			return;
		}
	}
	ResourceMesh* mesh = App->resources->GetPrimitive(PrimitiveType::CUBE);
	Gizmos::active_gizmos.push_back(Gizmo(mesh, true, PrimitiveType::CUBE));
	float4x4 matrix = float4x4::identity();
	matrix = matrix.FromTRS(centered_pos, Quat::identity(), size);
	DrawWire(mesh, matrix, color, line_width);
}

void Gizmos::DrawSphere(const float3& position, const float& radius, const Color& color)
{
	for (uint i = 0; i < Gizmos::active_gizmos.size(); ++i) {
		if (Gizmos::active_gizmos[i].type == PrimitiveType::SPHERE_ALIEN) {
			float4x4 matrix = float4x4::identity();
			matrix = matrix.FromTRS(position, Quat::identity(), { radius * 2, radius * 2, radius * 2 });
			DrawPoly(Gizmos::active_gizmos[i].mesh, matrix, color);
			Gizmos::active_gizmos[i].controller = controller;
			return;
		}
	}
	ResourceMesh* mesh = App->resources->GetPrimitive(PrimitiveType::SPHERE_ALIEN);
	Gizmos::active_gizmos.push_back(Gizmo(mesh, true, PrimitiveType::SPHERE_ALIEN));
	float4x4 matrix = float4x4::identity();
	matrix = matrix.FromTRS(position, Quat::identity(), { radius * 2, radius * 2, radius * 2 });
	DrawPoly(mesh, matrix, color);
}

void Gizmos::DrawWireSphere(const float3& position, const float& radius, const Color& color, float line_width)
{
	for (uint i = 0; i < Gizmos::active_gizmos.size(); ++i) {
		if (Gizmos::active_gizmos[i].type == PrimitiveType::SPHERE_ALIEN) {
			float4x4 matrix = float4x4::identity();
			matrix = matrix.FromTRS(position, Quat::identity(), { radius * 2, radius * 2, radius * 2 });
			DrawWire(Gizmos::active_gizmos[i].mesh, matrix, color, line_width);
			Gizmos::active_gizmos[i].controller = controller;
			return;
		}
	}
	ResourceMesh* mesh = App->resources->GetPrimitive(PrimitiveType::SPHERE_ALIEN);
	Gizmos::active_gizmos.push_back(Gizmo(mesh, true, PrimitiveType::SPHERE_ALIEN));
	float4x4 matrix = float4x4::identity();
	matrix = matrix.FromTRS(position, Quat::identity(), { radius * 2, radius * 2, radius * 2 });
	DrawWire(mesh, matrix, color, line_width);
}

void Gizmos::DrawLine(const float3& from, const float3& to, const Color& color, float line_width)
{
	glColor4f(color.r, color.g, color.b, color.a);
	glLineWidth(line_width);
	glBegin(GL_LINES);
	glVertex3f(from.x, from.y, from.z);
	glVertex3f(to.x, to.y, to.z);
	glEnd();
	glLineWidth(1);
}

void Gizmos::DrawWireMesh(const ComponentMesh * mesh, const float4x4& global_transform, const Color& color, float line_width)
{
	if (mesh != nullptr && mesh->mesh != nullptr) {
		DrawWire(mesh->mesh, global_transform, color, line_width);
	}
	else {
		LOG("DrawWireMesh: Mesh is nullptr or has no mesh attached!");
	}
}

void Gizmos::DrawPoly(const ResourceMesh * mesh, const float4x4& matrix, const Color& color)
{
	glColor4f(color.r, color.g, color.b, color.a);
	glPushMatrix();
	glMultMatrixf(matrix.Transposed().ptr());

	glEnableClientState(GL_VERTEX_ARRAY);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 0.1f);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	if (mesh->normals != nullptr) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glNormalPointer(GL_FLOAT, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glDrawElements(GL_TRIANGLES, mesh->num_index * 3, GL_UNSIGNED_INT, 0);

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}

void Gizmos::DrawWire(const ResourceMesh * mesh, const float4x4& matrix, const Color& color, const float& line_width)
{
	glPushMatrix();
	glMultMatrixf(matrix.Transposed().ptr());

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glLineWidth(line_width);
	glColor4f(color.r, color.g, color.b, color.a);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, mesh->num_index * 3, GL_UNSIGNED_INT, NULL);

	glLineWidth(1);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

void Gizmos::RemoveGizmos()
{
	std::vector<Gizmo>::iterator item = active_gizmos.begin();
	while (item != active_gizmos.end()) {
		if ((*item).controller != controller) {
			if ((*item).mesh != nullptr) {
				(*item).mesh->DecreaseReferences();
			}
			item = active_gizmos.erase(item);
		}
		else {
			++item;
		}
	}
}

void Gizmos::ClearAllCurrentGizmos()
{
	std::vector<Gizmo>::iterator item = active_gizmos.begin();
	while (item != active_gizmos.end()) {
		if ((*item).mesh != nullptr) {
			(*item).mesh->DecreaseReferences();
		}
		item = active_gizmos.erase(item);
	}
	active_gizmos.clear();
}
