#include "Objects.h"
#include "glew/include/glew.h"

Object::Object()
{

}

Object::~Object()
{

	glDeleteBuffers(num_vertex, &id_vertex);
	glDeleteBuffers(num_index, &id_index);

	delete[] vertex;
	delete[] index;

	vertex = nullptr;
	index = nullptr;
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

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, num_index * 3, GL_UNSIGNED_SHORT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
}

vec3 Object::GetPosition()
{
	return position;
}

