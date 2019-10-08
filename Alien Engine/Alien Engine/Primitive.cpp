#include "Primitive.h"
#include "Shapes.h"
#include "glew/include/glew.h"

Primitive::Primitive() : Object()
{
}

Primitive::~Primitive()
{
	par_shapes_free_mesh(shape);
}

void Primitive::InitBuffers()
{
	// buffer points
	glGenBuffers(1, &id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * shape->npoints * 3, shape->points, GL_STATIC_DRAW);

	// buffer index
	glGenBuffers(1, &id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * shape->ntriangles * 3, shape->triangles, GL_STATIC_DRAW);
}

void Primitive::MemCpy()
{

	num_vertex = shape->npoints;
	num_index = shape->ntriangles;

	vertex = new float[num_vertex * 3];
	index = new uint[num_index * 3];

	memcpy(vertex, shape->points, sizeof(float) * num_vertex * 3);
	memcpy(index, shape->triangles, sizeof(PAR_SHAPES_T) * num_index * 3);

}

void Primitive::RestartBuffers()
{
	delete[] vertex;
	delete[] index;
	MemCpy();
	InitBuffers();
}

void Primitive::SetPosition(const float& position_x, const float& position_y, const float& position_z)
{
	par_shapes_translate(shape, position_x, position_y, position_z);
}

void Primitive::SetSubdivisions(const int & subdivisions)
{
	par_shapes_free_mesh(shape);
	RestartBuffers();
	this->subdivisions = subdivisions;
	if (type == PrimitiveType::SPHERE_ALIEN)
	{
		par_shapes_create_subdivided_sphere(subdivisions);
	}
	else if (type == PrimitiveType::ROCK)
	{
		par_shapes_create_rock((static_cast<uint>(shape)->seed,subdivisions);
	}
}

