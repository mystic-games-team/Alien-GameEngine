#include "Primitive.h"
#include "Shapes.h"
#include "glew/include/glew.h"

#include "Rock.h"

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

	// buffer uv
	glGenBuffers(1, &id_uv);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_uv);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * num_vertex * 3, uv_cords, GL_STATIC_DRAW);

	// normals
	glGenBuffers(1, &id_normals);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_normals);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) *num_vertex * 3, &normals[0], GL_STATIC_DRAW);
}

void Primitive::MemCpy()
{
	num_vertex = shape->npoints;
	num_index = shape->ntriangles * 3;

	vertex = new float[num_vertex * 3];
	index = new uint[num_index * 3];

	memcpy(vertex, shape->points, sizeof(float) * num_vertex * 3);
	memcpy(index, shape->triangles, sizeof(PAR_SHAPES_T) * num_index);

	if (shape->tcoords != nullptr) {
		uv_cords = new float[num_vertex * 3];
		memcpy(uv_cords, shape->tcoords, sizeof(float) * num_vertex * 3);
	}

	if (shape->normals != nullptr) {
		normals = new float[num_vertex * 3];

		memcpy(normals, shape->normals, sizeof(float) * num_vertex * 3);

		center_point_normal = new float[shape->ntriangles * 3];
		center_point = new float[shape->ntriangles * 3];
		num_faces = shape->ntriangles;
		for (uint i = 0; i < num_index; i += 3)
		{
			uint index1 = index[i] * 3;
			uint index2 = index[i + 1] * 3;
			uint index3 = index[i + 2] * 3;

			vec3 x0(vertex[index1], vertex[index1 + 1], vertex[index1 + 2]);
			vec3 x1(vertex[index2], vertex[index2 + 1], vertex[index2 + 2]);
			vec3 x2(vertex[index3], vertex[index3 + 1], vertex[index3 + 2]);

			vec3 v0 = x0 - x2;
			vec3 v1 = x1 - x2;
			vec3 n = cross(v0, v1);

			vec3 normalized = normalize(n);

			center_point[i] = (x0.x + x1.x + x2.x) / 3;
			center_point[i + 1] = (x0.y + x1.y + x2.y) / 3;
			center_point[i + 2] = (x0.z + x1.z + x2.z) / 3;

			center_point_normal[i] = normalized.x;
			center_point_normal[i + 1] = normalized.y;
			center_point_normal[i + 2] = normalized.z;
		}
	}
	
}

void Primitive::RestartBuffers()
{
	glDeleteBuffers(1, &id_vertex);

	if (shape->tcoords != nullptr) {
		glDeleteBuffers(1, &id_uv);
		delete[] uv_cords;
	}
	if (shape->normals != nullptr) {
		glDeleteBuffers(1, &id_normals);
		delete[] normals;
	}
	
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
	this->subdivisions = subdivisions;
	if (type == PrimitiveType::SPHERE_ALIEN)
	{
		par_shapes_create_subdivided_sphere(subdivisions);
	}
	else if (type == PrimitiveType::ROCK)
	{
		par_shapes_create_rock((static_cast<Rock*>(this)->seed), subdivisions);
	}
	else
		assert("No type allowed");

	RestartBuffers();
}

