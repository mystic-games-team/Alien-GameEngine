#include "Primitive.h"
#include "Shapes.h"
#include "glew/include/glew.h"

Primitive::Primitive(const float& position_x, const float& position_y, const float& position_z) : Object(position_x,position_y,position_z)
{
}

Primitive::~Primitive()
{
	par_shapes_free_mesh(shape);
}

void Primitive::InitBuffers()
{
	// buffer points
	glGenBuffers(1, &iter_id);
	glBindBuffer(GL_ARRAY_BUFFER, iter_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * shape->npoints * 3, shape->points, GL_STATIC_DRAW);

	// buffer index
	glGenBuffers(1, &iter_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iter_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * shape->ntriangles * 3, shape->triangles, GL_STATIC_DRAW);
}
