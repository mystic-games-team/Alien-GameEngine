#include "Color.h"
#include "Objects.h"
#include "glew/include/glew.h"

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

void Object::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	SetColor(color);
	glColor3f(color.r, color.g, color.b);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
}

vec3 Object::GetPosition()
{
	return position;
}

void Object::SetColor(Color color)
{
	this->color = color;
}



