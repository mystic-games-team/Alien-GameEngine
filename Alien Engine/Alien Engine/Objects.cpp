#include "Color.h"
#include "Objects.h"
#include "glew/include/glew.h"
#include "ModuleObjects.h"
#include "Application.h"

Object::Object()
{

}

Object::~Object()
{

	glDeleteBuffers(1, &id_vertex);
	glDeleteBuffers(1, &id_index);
	glDeleteBuffers(1, &id_uv);
	glDeleteBuffers(1, &id_normals);

	delete[] index;
	delete[] vertex;
	delete[] normals;
	delete[] uv_cords;
	delete[] center_point_normal;
	delete[] center_point;

	center_point_normal = nullptr;
	center_point = nullptr;
	index = nullptr;
	vertex = nullptr;
	normals = nullptr;
	uv_cords = nullptr;
}

void Object::ChangeEnable()
{
	enabled = !enabled;
}

bool Object::IsEnabled()
{
	return enabled;
}

void Object::DrawPolygon()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	if (id_texture != -1) {
		// enable textures
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, id_texture);
		// set UV
		glBindBuffer(GL_ARRAY_BUFFER, id_uv);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		
	}
	glColor3f(color.r, color.g, color.b);

	if (normals != nullptr) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 0.1f);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, NULL);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Object::DrawMesh()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void Object::DrawVertexNormals()
{
	if (normals != nullptr) {
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
	}
}

void Object::DrawFaceNormals()
{
	if (normals != nullptr) {
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
	}
}

vec3 Object::GetPosition()
{
	return position;
}

void Object::SetColor(Color color)
{
	this->color = color;
}



