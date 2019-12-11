#include "Gizmos.h"
#include "Application.h"
#include "ResourceMesh.h"

std::vector<Gizmos::Gizmo> Gizmos::active_gizmos;

void Gizmos::DrawCube(float3 position, float3 size, Color color)
{
	float3 centered_pos = { position.x - size.x / 2 ,  position.y - size.y / 2 ,  position.z - size.z / 2 };
	for (uint i = 0; i < Gizmos::active_gizmos.size(); ++i) {
		if (Gizmos::active_gizmos[i].type == PrimitiveType::CUBE) {
			float4x4 matrix = float4x4::identity;
			matrix = matrix.FromTRS(centered_pos, Quat::identity, size);
			DrawPoly(Gizmos::active_gizmos[i].mesh, matrix, color);
			Gizmos::active_gizmos[i].is_in_use = true;
			return;
		}
	}
	ResourceMesh* mesh = App->resources->GetPrimitive(PrimitiveType::CUBE);
	Gizmos::active_gizmos.push_back({ mesh, true, PrimitiveType::CUBE });
	float4x4 matrix = float4x4::identity;
	matrix = matrix.FromTRS(centered_pos, Quat::identity, size);
	DrawPoly(mesh, matrix, color);
}

void Gizmos::DrawPoly(ResourceMesh* mesh, const float4x4& matrix, const Color& color)
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
